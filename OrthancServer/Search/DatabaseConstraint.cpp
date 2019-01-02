/**
 * Orthanc - A Lightweight, RESTful DICOM Store
 * Copyright (C) 2012-2016 Sebastien Jodogne, Medical Physics
 * Department, University Hospital of Liege, Belgium
 * Copyright (C) 2017-2019 Osimis S.A., Belgium
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


#include "../PrecompiledHeadersServer.h"
#include "DatabaseConstraint.h"

#include "../../Core/OrthancException.h"
#include "../ServerToolbox.h"

namespace Orthanc
{
  DatabaseConstraint::DatabaseConstraint(const DicomTagConstraint& constraint,
                                         ResourceType level,
                                         DicomTagType tagType) :
    level_(level),
    tag_(constraint.GetTag()),
    constraintType_(constraint.GetConstraintType()),
    mandatory_(constraint.IsMandatory())
  {
    switch (tagType)
    {
      case DicomTagType_Identifier:
        isIdentifier_ = true;
        caseSensitive_ = true;
        break;

      case DicomTagType_Main:
        isIdentifier_ = false;
        caseSensitive_ = constraint.IsCaseSensitive();
        break;

      default:
        throw OrthancException(ErrorCode_InternalError);
    }

    values_.reserve(constraint.GetValues().size());
      
    for (std::set<std::string>::const_iterator
           it = constraint.GetValues().begin();
         it != constraint.GetValues().end(); ++it)
    {
      if (isIdentifier_)
      {
        values_.push_back(ServerToolbox::NormalizeIdentifier(*it));
      }
      else
      {
        values_.push_back(*it);
      }
    }
  }

  
  const std::string& DatabaseConstraint::GetValue(size_t index) const
  {
    if (index >= values_.size())
    {
      throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
    else
    {
      return values_[index];
    }
  }


  const std::string& DatabaseConstraint::GetSingleValue() const
  {
    if (values_.size() != 1)
    {
      throw OrthancException(ErrorCode_BadSequenceOfCalls);
    }
    else
    {
      return values_[0];
    }
  }
}