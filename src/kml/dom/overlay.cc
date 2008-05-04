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

// This file contains the implementation of the abstract Overlay and concrete
// LatLonBox, GroundOverlay, ViewVolume, ImagePyramid, PhotoOverlay, OverlayXY,
// ScreenXY, RotationXY, Size, and ScreenOverlay elements.

#include "kml/dom/overlay.h"
#include "kml/dom/attributes.h"
#include "kml/dom/geometry.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

namespace kmldom {

Overlay::Overlay()
  : color_("ffffffff"),
    has_color_(false),
    draworder_(0),
    has_draworder_(false) {
}

Overlay::~Overlay() {}

void Overlay::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_color:
      has_color_ = element->SetString(&color_);
      break;
    case Type_drawOrder:
      has_draworder_ = element->SetInt(&draworder_);
      break;
    case Type_Icon:
      set_icon(AsIcon(element));
      break;
    default:
      Feature::AddElement(element);
      break;
  }
}

void Overlay::Serialize(Serializer& serializer) const {
  Feature::Serialize(serializer);
  if (has_color()) {
    serializer.SaveFieldById(Type_color, color());
  }
  if (has_draworder()) {
    serializer.SaveFieldById(Type_drawOrder, draworder());
  }
  if (has_icon()) {
    serializer.SaveElement(*icon());
  }
}

LatLonBox::LatLonBox()
  : rotation_(0.0),
    has_rotation_(false) {
}

LatLonBox::~LatLonBox() {
}

void LatLonBox::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_rotation:
      has_rotation_ = element->SetDouble(&rotation_);
      break;
    default:
      AbstractLatLonBox::AddElement(element);
      break;
  }
}

void LatLonBox::Serialize(Serializer& serializer) const {
  Attributes attributes;
  AbstractLatLonBox::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  AbstractLatLonBox::Serialize(serializer);
  if (has_rotation()) {
    serializer.SaveFieldById(Type_rotation, rotation());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

GroundOverlay::GroundOverlay()
  : altitude_(0.0),
    has_altitude_(false),
    altitudemode_(ALTITUDEMODE_CLAMPTOGROUND),
    has_altitudemode_(false) {
}

GroundOverlay::~GroundOverlay() {
}

void GroundOverlay::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_altitude:
      has_altitude_ = element->SetDouble(&altitude_);
      break;
    case Type_LatLonBox:
      set_latlonbox(AsLatLonBox(element));
      break;
    default:
      Overlay::AddElement(element);
      break;
  }
}

void GroundOverlay::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Overlay::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Overlay::Serialize(serializer);
  if (has_altitude()) {
    serializer.SaveFieldById(Type_altitude, altitude());
  }
  if (has_latlonbox()) {
    serializer.SaveElement(*latlonbox());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

ViewVolume::ViewVolume()
  : leftfov_(0.0),
    has_leftfov_(false),
    rightfov_(0.0),
    has_rightfov_(false),
    bottomfov_(0.0),
    has_bottomfov_(false),
    topfov_(0.0),
    has_topfov_(false),
    near_(0.0),
    has_near_(false) {
}

ViewVolume::~ViewVolume() {}

void ViewVolume::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_leftFov:
      has_leftfov_ = element->SetDouble(&leftfov_);
      break;
    case Type_rightFov:
      has_rightfov_ = element->SetDouble(&rightfov_);
      break;
    case Type_bottomFov:
      has_bottomfov_ = element->SetDouble(&bottomfov_);
      break;
    case Type_topFov:
      has_topfov_ = element->SetDouble(&topfov_);
      break;
    case Type_near:
      has_near_ = element->SetDouble(&near_);
      break;
    default:
      Object::AddElement(element);
      break;
  }
}

void ViewVolume::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  if (has_leftfov()) {
    serializer.SaveFieldById(Type_leftFov, leftfov());
  }
  if (has_rightfov()) {
    serializer.SaveFieldById(Type_rightFov, rightfov());
  }
  if (has_bottomfov()) {
    serializer.SaveFieldById(Type_bottomFov, bottomfov());
  }
  if (has_topfov()) {
    serializer.SaveFieldById(Type_topFov, topfov());
  }
  if (has_near()) {
    serializer.SaveFieldById(Type_near, near());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

ImagePyramid::ImagePyramid()
  : tilesize_(256),
    has_tilesize_(false),
    maxwidth_(0),
    has_maxwidth_(false),
    maxheight_(0),
    has_maxheight_(false),
    gridorigin_(GRIDORIGIN_LOWERLEFT),
    has_gridorigin_(false) {
}

ImagePyramid::~ImagePyramid() {}

void ImagePyramid::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_tileSize:
      has_tilesize_ = element->SetInt(&tilesize_);
      break;
    case Type_maxWidth:
      has_maxwidth_ = element->SetInt(&maxwidth_);
      break;
    case Type_maxHeight:
      has_maxheight_ = element->SetInt(&maxheight_);
      break;
    case Type_gridOrigin:
      has_gridorigin_ = element->SetEnum(&gridorigin_);
      break;
    default:
      Object::AddElement(element);
      break;
  }
}

void ImagePyramid::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  if (has_tilesize()) {
    serializer.SaveFieldById(Type_tileSize, tilesize());
  }
  if (has_maxwidth()) {
    serializer.SaveFieldById(Type_maxWidth, maxwidth());
  }
  if (has_maxheight()) {
    serializer.SaveFieldById(Type_maxHeight, maxheight());
  }
  if (has_gridorigin()) {
    serializer.SaveEnum(Type_gridOrigin, gridorigin());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

PhotoOverlay::PhotoOverlay()
  : rotation_(0.0),
    has_rotation_(false),
    shape_(SHAPE_RECTANGLE),
    has_shape_(false) {
}

PhotoOverlay::~PhotoOverlay() {
}

void PhotoOverlay::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_rotation:
      has_rotation_ = element->SetDouble(&rotation_);
      break;
    case Type_ViewVolume:
      set_viewvolume(AsViewVolume(element));
      break;
    case Type_ImagePyramid:
      set_imagepyramid(AsImagePyramid(element));
      break;
    case Type_Point:
      set_point(AsPoint(element));
      break;
    case Type_shape:
      has_shape_ = element->SetEnum(&shape_);
      break;
    default:
      Overlay::AddElement(element);
      break;
  }
}

void PhotoOverlay::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Overlay::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Overlay::Serialize(serializer);
  if (has_rotation()) {
    serializer.SaveFieldById(Type_rotation, rotation());
  }
  if (has_point()) {
    serializer.SaveElement(*point());
  }
  if (has_viewvolume()) {
    serializer.SaveElement(*viewvolume());
  }
  if (has_imagepyramid()) {
    serializer.SaveElement(*imagepyramid());
  }
  if (has_shape()) {
    serializer.SaveEnum(Type_shape, shape());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

OverlayXY::OverlayXY() {}

OverlayXY::~OverlayXY() {}

ScreenXY::ScreenXY() {}

ScreenXY::~ScreenXY() {}

RotationXY::RotationXY() {}

RotationXY::~RotationXY() {}

Size::Size() {}

Size::~Size() {}

ScreenOverlay::ScreenOverlay()
  : rotation_(0.0),
    has_rotation_(false) {
}

ScreenOverlay::~ScreenOverlay() {
}

void ScreenOverlay::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_overlayXY:
      set_overlayxy(AsOverlayXY(element));
      break;
    case Type_screenXY:
      set_screenxy(AsScreenXY(element));
      break;
    case Type_rotationXY:
      set_rotationxy(AsRotationXY(element));
      break;
    case Type_size:
      set_size(AsSize(element));
      break;
    case Type_rotation:
      has_rotation_ = element->SetDouble(&rotation_);
      break;
    default:
      Overlay::AddElement(element);
      break;
  }
}

void ScreenOverlay::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Overlay::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Overlay::Serialize(serializer);
  if (has_overlayxy()) {
    serializer.SaveElement(*overlayxy());
  }
  if (has_screenxy()) {
    serializer.SaveElement(*screenxy());
  }
  if (has_rotationxy()) {
    serializer.SaveElement(*rotationxy());
  }
  if (has_size()) {
    serializer.SaveElement(*size());
  }
  if (has_rotation()) {
    serializer.SaveFieldById(Type_rotation, rotation());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
