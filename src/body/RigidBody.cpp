/********************************************************************************
* ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/      *
* Copyright (c) 2010 Daniel Chappuis                                            *
*********************************************************************************
*                                                                               *
* Permission is hereby granted, free of charge, to any person obtaining a copy  *
* of this software and associated documentation files (the "Software"), to deal *
* in the Software without restriction, including without limitation the rights  *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
* copies of the Software, and to permit persons to whom the Software is         *
* furnished to do so, subject to the following conditions:                      *
*                                                                               *
* The above copyright notice and this permission notice shall be included in    *
* all copies or substantial portions of the Software.                           *
*                                                                               *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     *
* THE SOFTWARE.                                                                 *
********************************************************************************/

// Libraries
#include "RigidBody.h"
#include "BroadBoundingVolume.h"
#include "NarrowBoundingVolume.h"

 // We want to use the ReactPhysics3D namespace
 using namespace reactphysics3d;

 // Constructor
 RigidBody::RigidBody(const Vector3D& position, const Quaternion& orientation, double mass, const Matrix3x3& inertiaTensorLocal,
                      NarrowBoundingVolume* narrowBoundingVolume)
           : Body(mass), position(position), orientation(orientation.getUnit()), inertiaTensorLocal(inertiaTensorLocal),
             inertiaTensorLocalInverse(inertiaTensorLocal.getInverse()), massInverse(1.0/mass), oldPosition(position), oldOrientation(orientation) {

    restitution = 1.0;
    isMotionEnabled = true;
    isCollisionEnabled = true;
    interpolationFactor = 0.0;

    // Set the bounding volume for the narrow-phase collision detection
    setNarrowBoundingVolume(narrowBoundingVolume);

    // Compute the broad-phase bounding volume (an AABB)
    setBroadBoundingVolume(narrowBoundingVolume->computeAABB());

    // Update the orientation of the OBB according to the orientation of the rigid body
    update();

    assert(broadBoundingVolume);
    assert(narrowBoundingVolume);
}

// Destructor
RigidBody::~RigidBody() {

};

// Update the rigid body in order to reflect a change in the body state
void RigidBody::update() {
    // Update the orientation of the corresponding bounding volumes of the rigid body
    broadBoundingVolume->update(position, orientation);
    narrowBoundingVolume->update(position, orientation);
}