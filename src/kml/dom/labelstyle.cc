// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "kml/dom/labelstyle.h"
#include "kml/dom/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/serializer.h"

namespace kmldom {

LabelStyle::LabelStyle() :
  scale_(1.0),
  has_scale_(false) {
}

LabelStyle::~LabelStyle() {
}

void LabelStyle::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_scale:
      has_scale_ = element->SetDouble(&scale_);
      break;
    default:
      ColorStyle::AddElement(element);
      break;
  }
}

void LabelStyle::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  ColorStyle::Serialize(serializer);
  if (has_scale()) {
    serializer.SaveFieldById(Type_scale, scale());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
