// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SENSOR
#define SSVSC_SENSOR

#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Body/Base.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Utils/Utils.h"

namespace ssvsc
{
	class World;

	class Sensor : public Base
	{
		private:
			AABB shape;

		public:
			Sensor(World& mWorld, const Vec2i& mPosition, const Vec2i& mSize) : Base(mWorld), shape{mPosition, mSize / 2} { spatialInfo.preUpdate(); }

			inline void update(float mFrameTime) override
			{
				onPreUpdate();
				if(outOfBounds) { outOfBounds = false; return; };
				spatialInfo.preUpdate();
				spatialInfo.handleCollisions(mFrameTime);
				spatialInfo.postUpdate();
			}
			inline void handleCollision(float mFrameTime, Body* mBody) override
			{
				if(!mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;
				onDetection({*mBody, mBody->getUserData(), Utils::getMinIntersection(shape, mBody->getShape()), mFrameTime});
			}
			inline void destroy() override { spatialInfo.destroy(); Base::destroy(); }

			inline void setPosition(const Vec2i& mPosition)
			{
				if(mPosition != shape.getPosition()) spatialInfo.invalidate();
				shape.setPosition(mPosition);
			}

			inline AABB& getShape() override	{ return shape; }
			inline AABB& getOldShape() override	{ return shape; }
			inline BaseType getType() override	{ return BaseType::Sensor; }
	};
}

#endif

