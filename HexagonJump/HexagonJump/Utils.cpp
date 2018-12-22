#include "Utils.hpp"

namespace hexagon {

sf::ConvexShape CountTriangleCoords(float width, float height, Direction topVertexDirection)
{
	sf::ConvexShape shape(3);
	float dx = 0.f, dy = 0.f;

	switch (topVertexDirection)
	{
	case Direction::UP:
		dx = 0, dy = -1;
		break;
	case Direction::DOWN:
		dx = 0, dy = 1;
		break;
	case Direction::LEFT:
		dx = -1, dy = 0;
		break;
	case Direction::RIGHT:
		dx = 1, dy = 0;
		break;
	}

	shape.setPoint(0, sf::Vector2f(-dy * width / 2, -dx * width / 2));
	shape.setPoint(1, sf::Vector2f(dy * width / 2, dx * width / 2));
	shape.setPoint(2, sf::Vector2f(dx * height, dy * height));
	return shape;
}

/*/*function PointInRect(x, y, r)
	return x >= r.cx-r.w/2 and x <= r.cx+r.w/2 and y >= r.cy-r.h/2 and y <= r.cy+r.h/2
end

function RectRectCollision(r1, r2)
	return PointInRect(r1.cx-r1.w/2, r1.cy-r1.h/2, r2)
		or PointInRect(r1.cx+r1.w/2, r1.cy-r1.h/2, r2)
		or PointInRect(r1.cx+r1.w/2, r1.cy+r1.h/2, r2)
		or PointInRect(r1.cx-r1.w/2, r1.cy+r1.h/2, r2)
		or PointInRect(r2.cx-r2.w/2, r2.cy-r2.h/2, r1)
		or PointInRect(r2.cx+r2.w/2, r2.cy-r2.h/2, r1)
		or PointInRect(r2.cx+r2.w/2, r2.cy+r2.h/2, r1)
		or PointInRect(r2.cx-r2.w/2, r2.cy+r2.h/2, r1)
end

function RectRectAbsoluteXDistance(r1, r2)
	return math.abs(r1.cx - r2.cx) - r1.w/2 - r2.w/2
end

function RectRectAbsoluteYDistance(r1, r2)
	return math.abs(r1.cy - r2.cy) - r1.h/2 - r2.h/2
end

function RectRectDistanceAfterMovement(movRect, staticRect, dist, dir)
	local returnDist = 0
	local x, y = movRect.cx, movRect.cy
	
	if dir == "up" then
		movRect.cy = movRect.cy - dist
		
		if RectRectCollision(movRect, staticRect) then
			returnDist = RectRectAbsoluteYDistance(movRect, staticRect)
		end
	elseif dir == "down" then
		movRect.cy = movRect.cy + dist
		
		if RectRectCollision(movRect, staticRect) then
			returnDist = RectRectAbsoluteYDistance(movRect, staticRect)
		end
	elseif dir == "left" then
		movRect.cx = movRect.cx - dist
		
		if RectRectCollision(movRect, staticRect) then
			returnDist = RectRectAbsoluteXDistance(movRect, staticRect)
		end
	elseif dir == "right" then
		movRect.cx = movRect.cx + dist
		
		if RectRectCollision(movRect, staticRect) then
			returnDist = RectRectAbsoluteXDistance(movRect, staticRect)
		end
	end
	
	movRect.cx = x
	movRect.cy = y
	
	return dist + returnDist
end*/

float RectangleAbsoluteXDistance(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
	return std::abs(rect1.left + rect1.width / 2 - (rect2.left + rect2.width / 2)) - rect1.width / 2 - rect2.width / 2;
}

float RectangleAbsoluteYDistance(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
	return std::abs(rect1.top + rect1.height / 2 - (rect2.top + rect2.height / 2)) - rect1.height / 2 - rect2.height / 2;
}

float RectangleDistanceAfterMovement(sf::FloatRect dynamicRect, const sf::FloatRect& staticRect, float distance, Direction direction)
{
	float returnDistance = 0.f;
	auto moved = MoveVectorInDirection(sf::Vector2f{ dynamicRect.left, dynamicRect.top }, distance, direction);

	dynamicRect.left = moved.x;
	dynamicRect.top = moved.y;

	switch (direction) {
	case Direction::DOWN:
	case Direction::UP:
		if (dynamicRect.intersects(staticRect)) {
			returnDistance = RectangleAbsoluteYDistance(dynamicRect, staticRect);
		}
		break;
	case Direction::LEFT:
	case Direction::RIGHT:
		if (dynamicRect.intersects(staticRect)) {
			returnDistance = RectangleAbsoluteXDistance(dynamicRect, staticRect);
		}
		break;
	}

	return distance + returnDistance;
}

sf::Vector2f MoveVectorInDirection(sf::Vector2f vec, float distance, Direction direction)
{
	switch (direction) {
	case Direction::DOWN:
		vec.y += distance;
		break;
	case Direction::UP:
		vec.y -= distance;
		break;
	case Direction::LEFT:
		vec.x -= distance;
		break;
	case Direction::RIGHT:
		vec.x += distance;
		break;
	}
	return vec;
}

int Random(int from, int to)
{
	static std::random_device rd;
	static std::mt19937 eng(rd());
	return std::uniform_int_distribution<>(from, to)(eng);
}

}