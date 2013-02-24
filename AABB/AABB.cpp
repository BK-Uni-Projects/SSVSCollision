// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "AABB.h"

using namespace sf;

namespace ssvsc
{
	AABB::AABB(Vector2i mPosition, Vector2i mHalfSize) : position{mPosition}, halfSize{mHalfSize} { }

	void AABB::move(sf::Vector2i mOffset) { position += mOffset; }

	void AABB::setPosition(Vector2i mPosition) 		{ position = mPosition; }
	void AABB::setX(int mX)				 			{ position.x = mX; }
	void AABB::setY(int mY)				 			{ position.y = mY; }
	void AABB::setHalfSize(Vector2i mHalfSize)		{ halfSize = mHalfSize; }
	void AABB::setSize(Vector2i mSize)				{ halfSize = mSize / 2; }
	void AABB::setWidth(int mWidth)					{ halfSize.x = mWidth / 2; }
	void AABB::setHeight(int mHeight)				{ halfSize.y = mHeight / 2; }

	Vector2i AABB::getPosition() const 				{ return position; }
	int AABB::getX() const 							{ return position.x; }
	int AABB::getY() const 							{ return position.y; }
	int AABB::getLeft() const 						{ return position.x - halfSize.x; }
	int AABB::getRight() const 						{ return position.x + halfSize.x; }
	int AABB::getTop() const 						{ return position.y - halfSize.y; }
	int AABB::getBottom() const 					{ return position.y + halfSize.y; }
	Vector2i AABB::getHalfSize() const 				{ return halfSize; }
	int AABB::getHalfWidth() const 					{ return halfSize.x; }
	int AABB::getHalfHeight() const 				{ return halfSize.y; }
	Vector2i AABB::getSize() const 					{ return halfSize * 2; }
	int AABB::getWidth() const 						{ return halfSize.x * 2; }
	int AABB::getHeight() const 					{ return halfSize.y * 2; }
	
	bool AABB::isLeftOf(const AABB& mAABB) const	{ return getRight() <= mAABB.getLeft(); }
	bool AABB::isRightOf(const AABB& mAABB) const	{ return getLeft() >= mAABB.getRight(); }
	bool AABB::isAbove(const AABB& mAABB) const		{ return getBottom() <= mAABB.getTop(); }
	bool AABB::isBelow(const AABB& mAABB) const		{ return getTop() >= mAABB.getBottom(); }

	bool AABB::operator==(const AABB& mOther) const { return position == mOther.position && halfSize == mOther.halfSize; }
}