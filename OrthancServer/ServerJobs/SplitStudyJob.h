/**
 * Orthanc - A Lightweight, RESTful DICOM Store
 * Copyright (C) 2012-2016 Sebastien Jodogne, Medical Physics
 * Department, University Hospital of Liege, Belgium
 * Copyright (C) 2017-2018 Osimis S.A., Belgium
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


#pragma once

#include "../../Core/JobsEngine/SetOfInstancesJob.h"

#include "../ServerContext.h"

namespace Orthanc
{
  class SplitStudyJob : public SetOfInstancesJob
  {
  private:
    typedef std::map<std::string, std::string>  SeriesUidMap;
    typedef std::map<DicomTag, std::string>     Replacements;
    
    
    ServerContext&         context_;
    bool                   keepSource_;
    std::string            sourceStudy_;
    std::set<std::string>  sourceSeries_;
    std::string            targetStudy_;
    std::string            targetStudyUid_;
    SeriesUidMap           targetSeries_;
    std::set<DicomTag>     allowedTags_;
    DicomInstanceOrigin    origin_;
    Replacements           replacements_;
    std::set<DicomTag>     removals_;

    void CheckAllowedTag(const DicomTag& tag) const;
    
    void Setup();
    
  protected:
    virtual bool HandleInstance(const std::string& instance);

    virtual bool HandleTrailingStep();
    
  public:
    SplitStudyJob(ServerContext& context,
                  const std::string& sourceStudy);

    SplitStudyJob(ServerContext& context,
                  const Json::Value& serialized);

    const std::string& GetSourceStudy() const
    {
      return sourceStudy_;
    }

    void AddSourceSeries(const std::string& series);

    bool IsKeepSource() const
    {
      return keepSource_;
    }
    
    void SetKeepSource(bool keep);

    void Replace(const DicomTag& tag,
                 const std::string& value);
    
    void Remove(const DicomTag& tag);
    
    void SetOrigin(const DicomInstanceOrigin& origin);

    void SetOrigin(const RestApiCall& call);

    const DicomInstanceOrigin& GetOrigin() const
    {
      return origin_;
    }

    virtual void Stop(JobStopReason reason)
    {
    }

    virtual void GetJobType(std::string& target)
    {
      target = "SplitStudy";
    }

    virtual void GetPublicContent(Json::Value& value);

    virtual bool Serialize(Json::Value& target);
  };
}
