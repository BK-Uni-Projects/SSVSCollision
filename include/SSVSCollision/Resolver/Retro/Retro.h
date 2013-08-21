// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_RESOLVER_RETRO
#define SSVSC_RESOLVER_RETRO

#include "SSVSCollision/Resolver/ResolverBase.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	class Body;

	struct Retro : public ResolverBase
	{
		void resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve) override
		{
			AABB& shape(mBody.getShape());
			const AABB& oldShape(mBody.getOldShape());
			ssvu::sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return Utils::getOverlapArea(shape, mA->getShape()) > Utils::getOverlapArea(shape, mB->getShape()); });

			for(const auto& b : mBodiesToResolve)
			{
				const AABB& s(b->getShape());
				const AABB& os(b->getOldShape());

				int iX{Utils::getMinIntersectionX(shape, s)}, iY{Utils::getMinIntersectionY(shape, s)};
				Vec2i resolution{std::abs(iX) < std::abs(iY) ? Vec2i{iX, 0} : Vec2i{0, iY}};

				bool noResolvePosition{false}, noResolveVelocity{false};
				mBody.onResolution({*b, b->getUserData(), {iX, iY}, resolution, noResolvePosition, noResolveVelocity});

				if(!noResolvePosition) shape.move(resolution);
				if(noResolveVelocity) continue;

				// Remember that shape has moved now
				bool oldShapeLeftOfS{oldShape.isLeftOf(s)}, oldShapeRightOfS{oldShape.isRightOf(s)};
				bool oldShapeAboveS{oldShape.isAbove(s)}, oldShapeBelowS{oldShape.isBelow(s)};
				bool oldHOverlap{!(oldShapeLeftOfS || oldShapeRightOfS)}, oldVOverlap{!(oldShapeAboveS || oldShapeBelowS)};

				constexpr float restitutionX{-0.f}; // TODO: make this customizable
				constexpr float restitutionY{-0.f}; // TODO: make this customizable
				const auto& velocity(mBody.getVelocity());

				if		(resolution.y < 0 && velocity.y > 0 && (oldShapeAboveS || (os.isBelow(shape) && oldHOverlap))) mBody.setVelocityY(velocity.y * restitutionY);
				else if	(resolution.y > 0 && velocity.y < 0 && (oldShapeBelowS || (os.isAbove(shape) && oldHOverlap))) mBody.setVelocityY(velocity.y * restitutionY);

				if		(resolution.x < 0 && velocity.x > 0 && (oldShapeLeftOfS || (os.isRightOf(shape) && oldVOverlap))) mBody.setVelocityX(velocity.x * restitutionX);
				else if	(resolution.x > 0 && velocity.x < 0 && (oldShapeRightOfS || (os.isLeftOf(shape) && oldVOverlap))) mBody.setVelocityX(velocity.x * restitutionX);
			}
		}
	};
}

#endif
