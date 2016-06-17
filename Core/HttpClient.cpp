/**
 * Orthanc - A Lightweight, RESTful DICOM Store
 * Copyright (C) 2012-2016 Sebastien Jodogne, Medical Physics
 * Department, University Hospital of Liege, Belgium
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * In addition, as a special exception, the copyright holders of this
 * program give permission to link the code of its release with the
 * OpenSSL project's "OpenSSL" library (or with modified versions of it
 * that use the same license as the "OpenSSL" library), and distribute
 * the linked executables. You must obey the GNU General Public License
 * in all respects for all of the code used other than "OpenSSL". If you
 * modify file(s) with this exception, you may extend this exception to
 * your version of the file(s), but you are not obligated to do so. If
 * you do not wish to do so, delete this exception statement from your
 * version. If you delete this exception statement from all source files
 * in the program, then also delete it here.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/


#include "PrecompiledHeaders.h"
#include "HttpClient.h"

#include "Toolbox.h"
#include "OrthancException.h"
#include "Logging.h"

#include <string.h>
#include <curl/curl.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/thread/mutex.hpp>


#if ORTHANC_PKCS11_ENABLED == 1

#include <openssl/engine.h>
#include <libp11.h>

// Include the "libengine-pkcs11-openssl" from the libp11 package
extern "C"
{
#pragma GCC diagnostic error "-fpermissive"
#include <libp11/eng_front.c>
}

#endif


extern "C"
{
  static CURLcode GetHttpStatus(CURLcode code, CURL* curl, long* status)
  {
    if (code == CURLE_OK)
    {
      code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, status);
      return code;
    }
    else
    {
      *status = 0;
      return code;
    }
  }

  // This is a dummy wrapper function to suppress any OpenSSL-related
  // problem in valgrind. Inlining is prevented.
#if defined(__GNUC__) || defined(__clang__)
    __attribute__((noinline)) 
#endif
    static CURLcode OrthancHttpClientPerformSSL(CURL* curl, long* status)
  {
    return GetHttpStatus(curl_easy_perform(curl), curl, status);
  }
}



namespace Orthanc
{
  class HttpClient::GlobalParameters
  {
  private:
    boost::mutex    mutex_;
    bool            httpsVerifyPeers_;
    std::string     httpsCACertificates_;
    std::string     proxy_;
    long            timeout_;
    bool            pkcs11Initialized_;

#if ORTHANC_PKCS11_ENABLED == 1
    static ENGINE* LoadPkcs11Engine()
    {
      // This function mimics the "ENGINE_load_dynamic" function from
      // OpenSSL, in file "crypto/engine/eng_dyn.c"

      ENGINE* engine = ENGINE_new();
      if (!engine)
      {
        LOG(ERROR) << "Cannot create an OpenSSL engine for PKCS11";
        throw OrthancException(ErrorCode_InternalError);
      }

      if (!bind_helper(engine) ||
          !ENGINE_add(engine))
      {
        LOG(ERROR) << "Cannot initialize the OpenSSL engine for PKCS11";
        ENGINE_free(engine);
        throw OrthancException(ErrorCode_InternalError);
      }

      // If the "ENGINE_add" worked, it gets a structural
      // reference. We release our just-created reference.
      ENGINE_free(engine);

      assert(!strcmp("pkcs11", PKCS11_ENGINE_ID));
      return ENGINE_by_id(PKCS11_ENGINE_ID);
    }
#endif

    GlobalParameters() : 
      httpsVerifyPeers_(true),
      timeout_(0),
      pkcs11Initialized_(false)
    {
    }

  public:
    // Singleton pattern
    static GlobalParameters& GetInstance()
    {
      static GlobalParameters parameters;
      return parameters;
    }

    void ConfigureSsl(bool httpsVerifyPeers,
                      const std::string& httpsCACertificates)
    {
      boost::mutex::scoped_lock lock(mutex_);
      httpsVerifyPeers_ = httpsVerifyPeers;
      httpsCACertificates_ = httpsCACertificates;
    }

    void GetSslConfiguration(bool& httpsVerifyPeers,
                             std::string& httpsCACertificates)
    {
      boost::mutex::scoped_lock lock(mutex_);
      httpsVerifyPeers = httpsVerifyPeers_;
      httpsCACertificates = httpsCACertificates_;
    }

    void SetDefaultProxy(const std::string& proxy)
    {
      LOG(INFO) << "Setting the default proxy for HTTP client connections: " << proxy;

      {
        boost::mutex::scoped_lock lock(mutex_);
        proxy_ = proxy;
      }
    }

    void GetDefaultProxy(std::string& target)
    {
      boost::mutex::scoped_lock lock(mutex_);
      target = proxy_;
    }

    void SetDefaultTimeout(long seconds)
    {
      LOG(INFO) << "Setting the default timeout for HTTP client connections: " << seconds << " seconds";

      {
        boost::mutex::scoped_lock lock(mutex_);
        timeout_ = seconds;
      }
    }

    long GetDefaultTimeout()
    {
      boost::mutex::scoped_lock lock(mutex_);
      return timeout_;
    }

    bool IsPkcs11Initialized()
    {
      boost::mutex::scoped_lock lock(mutex_);
      return pkcs11Initialized_;
    }


#if ORTHANC_PKCS11_ENABLED == 1
    void InitializePkcs11(const std::string& module,
                          const std::string& pin,
                          bool verbose)
    {
      boost::mutex::scoped_lock lock(mutex_);

      if (pkcs11Initialized_)
      {
        LOG(ERROR) << "The PKCS11 engine has already been initialized";
        throw OrthancException(ErrorCode_BadSequenceOfCalls);
      }

      if (module.empty() ||
          !Toolbox::IsRegularFile(module))
      {
        LOG(ERROR) << "The PKCS11 module must be a path to one shared library (DLL or .so)";
        throw OrthancException(ErrorCode_InexistentFile);
      }

      ENGINE* engine = LoadPkcs11Engine();
      if (!engine)
      {
        LOG(ERROR) << "Cannot create an OpenSSL engine for PKCS11";
        throw OrthancException(ErrorCode_InternalError);
      }

      if (!ENGINE_ctrl_cmd_string(engine, "MODULE_PATH", module.c_str(), 0))
      {
        LOG(ERROR) << "Cannot configure the OpenSSL dynamic engine for PKCS11";
        throw OrthancException(ErrorCode_InternalError);
      }

      if (verbose)
      {
        ENGINE_ctrl_cmd_string(engine, "VERBOSE", NULL, 0);
      }

      if (!pin.empty() &&
          !ENGINE_ctrl_cmd_string(engine, "PIN", pin.c_str(), 0)) 
      {
        LOG(ERROR) << "Cannot set the PIN code for PKCS11";
        throw OrthancException(ErrorCode_InternalError);
      }
  
      if (!ENGINE_init(engine))
      {
        LOG(ERROR) << "Cannot initialize the OpenSSL dynamic engine for PKCS11";
        throw OrthancException(ErrorCode_InternalError);
      }

      LOG(WARNING) << "The PKCS11 engine has been successfully initialized";
      pkcs11Initialized_ = true;
    }
#endif
  };


  struct HttpClient::PImpl
  {
    CURL* curl_;
    struct curl_slist *defaultPostHeaders_;
    struct curl_slist *userHeaders_;
  };


  static void ThrowException(HttpStatus status)
  {
    switch (status)
    {
      case HttpStatus_400_BadRequest:
        throw OrthancException(ErrorCode_BadRequest);

      case HttpStatus_401_Unauthorized:
      case HttpStatus_403_Forbidden:
        throw OrthancException(ErrorCode_Unauthorized);

      case HttpStatus_404_NotFound:
        throw OrthancException(ErrorCode_InexistentItem);

      default:
        throw OrthancException(ErrorCode_NetworkProtocol);
    }
  }



  static CURLcode CheckCode(CURLcode code)
  {
    if (code != CURLE_OK)
    {
      LOG(ERROR) << "libCURL error: " + std::string(curl_easy_strerror(code));
      throw OrthancException(ErrorCode_NetworkProtocol);
    }

    return code;
  }


  static size_t CurlCallback(void *buffer, size_t size, size_t nmemb, void *payload)
  {
    std::string& target = *(static_cast<std::string*>(payload));

    size_t length = size * nmemb;
    if (length == 0)
      return 0;

    size_t pos = target.size();

    target.resize(pos + length);
    memcpy(&target.at(pos), buffer, length);

    return length;
  }


  void HttpClient::Setup()
  {
    pimpl_->userHeaders_ = NULL;
    pimpl_->defaultPostHeaders_ = NULL;
    if ((pimpl_->defaultPostHeaders_ = curl_slist_append(pimpl_->defaultPostHeaders_, "Expect:")) == NULL)
    {
      throw OrthancException(ErrorCode_NotEnoughMemory);
    }

    pimpl_->curl_ = curl_easy_init();
    if (!pimpl_->curl_)
    {
      curl_slist_free_all(pimpl_->defaultPostHeaders_);
      throw OrthancException(ErrorCode_NotEnoughMemory);
    }

    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_WRITEFUNCTION, &CurlCallback));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_HEADER, 0));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_FOLLOWLOCATION, 1));

    // This fixes the "longjmp causes uninitialized stack frame" crash
    // that happens on modern Linux versions.
    // http://stackoverflow.com/questions/9191668/error-longjmp-causes-uninitialized-stack-frame
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_NOSIGNAL, 1));

    url_ = "";
    method_ = HttpMethod_Get;
    lastStatus_ = HttpStatus_200_Ok;
    isVerbose_ = false;
    timeout_ = GlobalParameters::GetInstance().GetDefaultTimeout();
    GlobalParameters::GetInstance().GetDefaultProxy(proxy_);
    GlobalParameters::GetInstance().GetSslConfiguration(verifyPeers_, caCertificates_);
  }


  HttpClient::HttpClient() : 
    pimpl_(new PImpl), 
    verifyPeers_(true),
    pkcs11Enabled_(false)
  {
    Setup();
  }


  HttpClient::HttpClient(const WebServiceParameters& service,
                         const std::string& uri) : 
    pimpl_(new PImpl), 
    verifyPeers_(true)
  {
    Setup();

    if (service.GetUsername().size() != 0 && 
        service.GetPassword().size() != 0)
    {
      SetCredentials(service.GetUsername().c_str(), 
                     service.GetPassword().c_str());
    }

    if (!service.GetCertificateFile().empty())
    {
      SetClientCertificate(service.GetCertificateFile(),
                           service.GetCertificateKeyFile(),
                           service.GetCertificateKeyPassword());
    }

    SetPkcs11Enabled(service.IsPkcs11Enabled());

    SetUrl(service.GetUrl() + uri);
  }


  HttpClient::~HttpClient()
  {
    curl_easy_cleanup(pimpl_->curl_);
    curl_slist_free_all(pimpl_->defaultPostHeaders_);
    ClearHeaders();
  }


  void HttpClient::SetVerbose(bool isVerbose)
  {
    isVerbose_ = isVerbose;

    if (isVerbose_)
    {
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_VERBOSE, 1));
    }
    else
    {
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_VERBOSE, 0));
    }
  }


  void HttpClient::AddHeader(const std::string& key,
                             const std::string& value)
  {
    if (key.empty())
    {
      throw OrthancException(ErrorCode_ParameterOutOfRange);
    }

    std::string s = key + ": " + value;

    if ((pimpl_->userHeaders_ = curl_slist_append(pimpl_->userHeaders_, s.c_str())) == NULL)
    {
      throw OrthancException(ErrorCode_NotEnoughMemory);
    }
  }


  void HttpClient::ClearHeaders()
  {
    if (pimpl_->userHeaders_ != NULL)
    {
      curl_slist_free_all(pimpl_->userHeaders_);
      pimpl_->userHeaders_ = NULL;
    }
  }


  bool HttpClient::Apply(std::string& answer)
  {
    answer.clear();
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_URL, url_.c_str()));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_WRITEDATA, &answer));

#if ORTHANC_SSL_ENABLED == 1
    // Setup HTTPS-related options

    if (verifyPeers_)
    {
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_CAINFO, caCertificates_.c_str()));
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSL_VERIFYHOST, 2));  // libcurl default is strict verifyhost
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSL_VERIFYPEER, 1)); 
    }
    else
    {
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSL_VERIFYHOST, 0)); 
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSL_VERIFYPEER, 0)); 
    }
#endif

    // Setup the HTTPS client certificate
    if (!clientCertificateFile_.empty() &&
        pkcs11Enabled_)
    {
      LOG(ERROR) << "Cannot enable both client certificates and PKCS#11 authentication";
      throw OrthancException(ErrorCode_ParameterOutOfRange);
    }

    if (pkcs11Enabled_)
    {
#if ORTHANC_PKCS11_ENABLED == 1
      if (GlobalParameters::GetInstance().IsPkcs11Initialized())
      {
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSLENGINE, "pkcs11"));
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSLKEYTYPE, "ENG"));
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSLCERTTYPE, "ENG"));
      }
      else
      {
        LOG(ERROR) << "Cannot use PKCS11 for a HTTPS request, because it has not been initialized";
        throw OrthancException(ErrorCode_BadSequenceOfCalls);
      }
#else
      LOG(ERROR) << "This version of Orthanc is compiled without support for PKCS11";
      throw OrthancException(ErrorCode_InternalError);
#endif
    }
    else if (!clientCertificateFile_.empty())
    {
#if ORTHANC_SSL_ENABLED == 1
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSLCERTTYPE, "PEM"));
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSLCERT, clientCertificateFile_.c_str()));

      if (!clientCertificateKeyPassword_.empty())
      {
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_KEYPASSWD, clientCertificateKeyPassword_.c_str()));
      }

      // NB: If no "clientKeyFile_" is provided, the key must be
      // prepended to the certificate file
      if (!clientCertificateKeyFile_.empty())
      {
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSLKEYTYPE, "PEM"));
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_SSLKEY, clientCertificateKeyFile_.c_str()));
      }
#else
      LOG(ERROR) << "This version of Orthanc is compiled without OpenSSL support, cannot use HTTPS client authentication";
      throw OrthancException(ErrorCode_InternalError);
#endif
    }

    // Reset the parameters from previous calls to Apply()
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_HTTPHEADER, pimpl_->userHeaders_));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_HTTPGET, 0L));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_POST, 0L));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_NOBODY, 0L));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_CUSTOMREQUEST, NULL));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_POSTFIELDS, NULL));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_POSTFIELDSIZE, 0));
    CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_PROXY, NULL));

    // Set timeouts
    if (timeout_ <= 0)
    {
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_TIMEOUT, 10));  /* default: 10 seconds */
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_CONNECTTIMEOUT, 10));  /* default: 10 seconds */
    }
    else
    {
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_TIMEOUT, timeout_));
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_CONNECTTIMEOUT, timeout_));
    }

    if (credentials_.size() != 0)
    {
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_USERPWD, credentials_.c_str()));
    }

    if (proxy_.size() != 0)
    {
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_PROXY, proxy_.c_str()));
    }

    switch (method_)
    {
    case HttpMethod_Get:
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_HTTPGET, 1L));
      break;

    case HttpMethod_Post:
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_POST, 1L));

      if (pimpl_->userHeaders_ == NULL)
      {
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_HTTPHEADER, pimpl_->defaultPostHeaders_));
      }

      break;

    case HttpMethod_Delete:
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_NOBODY, 1L));
      CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_CUSTOMREQUEST, "DELETE"));
      break;

    case HttpMethod_Put:
      // http://stackoverflow.com/a/7570281/881731: Don't use
      // CURLOPT_PUT if there is a body

      // CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_PUT, 1L));

      curl_easy_setopt(pimpl_->curl_, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */

      if (pimpl_->userHeaders_ == NULL)
      {
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_HTTPHEADER, pimpl_->defaultPostHeaders_));
      }

      break;

    default:
      throw OrthancException(ErrorCode_InternalError);
    }


    if (method_ == HttpMethod_Post ||
        method_ == HttpMethod_Put)
    {
      if (body_.size() > 0)
      {
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_POSTFIELDS, body_.c_str()));
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_POSTFIELDSIZE, body_.size()));
      }
      else
      {
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_POSTFIELDS, NULL));
        CheckCode(curl_easy_setopt(pimpl_->curl_, CURLOPT_POSTFIELDSIZE, 0));
      }
    }


    // Do the actual request
    CURLcode code;
    long status = 0;

    if (boost::starts_with(url_, "https://"))
    {
      code = OrthancHttpClientPerformSSL(pimpl_->curl_, &status);
    }
    else
    {
      code = GetHttpStatus(curl_easy_perform(pimpl_->curl_), pimpl_->curl_, &status);
    }

    CheckCode(code);

    if (status == 0)
    {
      // This corresponds to a call to an inexistent host
      lastStatus_ = HttpStatus_500_InternalServerError;
    }
    else
    {
      lastStatus_ = static_cast<HttpStatus>(status);
    }

    bool success = (status >= 200 && status < 300);

    if (!success)
    {
      LOG(INFO) << "Error in HTTP request, received HTTP status " << status 
                << " (" << EnumerationToString(lastStatus_) << ")";
    }

    return success;
  }


  bool HttpClient::Apply(Json::Value& answer)
  {
    std::string s;
    if (Apply(s))
    {
      Json::Reader reader;
      return reader.parse(s, answer);
    }
    else
    {
      return false;
    }
  }


  void HttpClient::SetCredentials(const char* username,
                                  const char* password)
  {
    credentials_ = std::string(username) + ":" + std::string(password);
  }


  void HttpClient::ConfigureSsl(bool httpsVerifyPeers,
                                const std::string& httpsVerifyCertificates)
  {
#if ORTHANC_SSL_ENABLED == 1
    if (httpsVerifyPeers)
    {
      if (httpsVerifyCertificates.empty())
      {
        LOG(WARNING) << "No certificates are provided to validate peers, "
                     << "set \"HttpsCACertificates\" if you need to do HTTPS requests";
      }
      else
      {
        LOG(WARNING) << "HTTPS will use the CA certificates from this file: " << httpsVerifyCertificates;
      }
    }
    else
    {
      LOG(WARNING) << "The verification of the peers in HTTPS requests is disabled";
    }
#endif

    GlobalParameters::GetInstance().ConfigureSsl(httpsVerifyPeers, httpsVerifyCertificates);
  }

  
  void HttpClient::GlobalInitialize()
  {
#if ORTHANC_SSL_ENABLED == 1
    CheckCode(curl_global_init(CURL_GLOBAL_ALL));
#else
    CheckCode(curl_global_init(CURL_GLOBAL_ALL & ~CURL_GLOBAL_SSL));
#endif

    CheckCode(curl_global_init(CURL_GLOBAL_DEFAULT));
  }


  void HttpClient::GlobalFinalize()
  {
    curl_global_cleanup();
  }
  

  void HttpClient::SetDefaultProxy(const std::string& proxy)
  {
    GlobalParameters::GetInstance().SetDefaultProxy(proxy);
  }


  void HttpClient::SetDefaultTimeout(long timeout)
  {
    GlobalParameters::GetInstance().SetDefaultTimeout(timeout);
  }


  void HttpClient::ApplyAndThrowException(std::string& answer)
  {
    if (!Apply(answer))
    {
      ThrowException(GetLastStatus());
    }
  }
  
  void HttpClient::ApplyAndThrowException(Json::Value& answer)
  {
    if (!Apply(answer))
    {
      ThrowException(GetLastStatus());
    }
  }


  void HttpClient::SetClientCertificate(const std::string& certificateFile,
                                        const std::string& certificateKeyFile,
                                        const std::string& certificateKeyPassword)
  {
    if (certificateFile.empty())
    {
      throw OrthancException(ErrorCode_ParameterOutOfRange);
    }

    if (!Toolbox::IsRegularFile(certificateFile))
    {
      LOG(ERROR) << "Cannot open certificate file: " << certificateFile;
      throw OrthancException(ErrorCode_InexistentFile);
    }

    if (!certificateKeyFile.empty() && 
        !Toolbox::IsRegularFile(certificateKeyFile))
    {
      LOG(ERROR) << "Cannot open key file: " << certificateKeyFile;
      throw OrthancException(ErrorCode_InexistentFile);
    }

    clientCertificateFile_ = certificateFile;
    clientCertificateKeyFile_ = certificateKeyFile;
    clientCertificateKeyPassword_ = certificateKeyPassword;
  }


  void HttpClient::InitializePkcs11(const std::string& module,
                                    const std::string& pin,
                                    bool verbose)
  {
#if ORTHANC_PKCS11_ENABLED == 1
    LOG(INFO) << "Initializing PKCS#11 using " << module 
              << (pin.empty() ? "(no PIN provided)" : "(PIN is provided)");
    GlobalParameters::GetInstance().InitializePkcs11(module, pin, verbose);    
#else
    LOG(ERROR) << "This version of Orthanc is compiled without support for PKCS11";
    throw OrthancException(ErrorCode_InternalError);
#endif
  }
}
