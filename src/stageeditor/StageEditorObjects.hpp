/**
 * @file StageEditorObjects.hpp
 * @author Tomáš Ludrovan
 * @brief Stage editor objects
 * @version 0.1
 * @date 2024-02-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEEDITOROBJECTS_HPP
#define STAGEEDITOROBJECTS_HPP

#include "types.hpp"
#include "math/Math.hpp"
#include "stageeditor/Common.hpp"

/**
 * @brief Provides definitions common to all editor objects.
 */
class StageEditorObjectBase {
private:
	EditorOID m_oid;
protected:
public:
	StageEditorObjectBase(EditorOID oid)
		: m_oid{oid}
	{}

	// Makes the class abstract. Definition is in the .cpp file
	virtual ~StageEditorObjectBase() = 0;

	virtual void moveBy(double dx, double dy) = 0;
	virtual bool containsPoint(const PointF& pt) const = 0;

	EditorOID getOid() const {
		return m_oid;
	}
};

class StageEditorPlayerObject : public StageEditorObjectBase {
public:
	PointF pos;

	StageEditorPlayerObject(EditorOID oid)
		: StageEditorObjectBase(oid)
	{}

	StageEditorPlayerObject(EditorOID oid, const PointF& pos)
		: StageEditorObjectBase(oid)
		, pos(pos)
	{}

	StageEditorPlayerObject(EditorOID oid, PointF&& pos)
		: StageEditorObjectBase(oid)
		, pos(pos)
	{}

	void moveBy(double dx, double dy) override {
		pos.x += dx;
		pos.y += dy;
	}

	constexpr double getRadius() const {
		return EDITOR_PLAYER_RADIUS;
	}

	bool containsPoint(const PointF& pt) const override {
		double sqrDist = this->pos.sqrDistance(pt);
		return (sqrDist <= sqr(this->getRadius()));
	}

	constexpr bool collidesWithPlayer(const StageEditorPlayerObject& player)
		const
	{
		double sqrMinDistance = sqr(this->getRadius() + player.getRadius());
		double sqrCurrDistance = this->pos.sqrDistance(player.pos);
		return (sqrCurrDistance < sqrMinDistance);
	}
};

class StageEditorObstacleObject : public StageEditorObjectBase {
public:
	PolygonF shape;

	StageEditorObstacleObject(EditorOID oid)
		: StageEditorObjectBase(oid)
	{}

	StageEditorObstacleObject(EditorOID oid, const PolygonF& shape)
		: StageEditorObjectBase(oid)
		, shape(shape)
	{}

	StageEditorObstacleObject(EditorOID oid, PolygonF&& shape)
		: StageEditorObjectBase(oid)
		, shape(shape)
	{}

	void moveBy(double dx, double dy) override {
		for (auto& corner : shape.corners) {
			corner.x += dx;
			corner.y += dy;
		}
	}

	bool containsPoint(const PointF& pt) const override {
		return shape.containsPoint(pt);
	}

	bool collidesWithPlayer(const StageEditorPlayerObject& player)
		const
	{
		double sqrMinDistance = sqr(player.getRadius());
		double sqrCurrDistance = this->shape.sqrDistance(player.pos);
		return (sqrCurrDistance < sqrMinDistance);
	}
};

#endif // STAGEEDITOROBJECTS_HPP
