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
	virtual ~StageEditorObjectBase() = 0;

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

	constexpr double getRadius() const {
		return EDITOR_PLAYER_RADIUS;
	}

	constexpr bool containsPoint(const PointF& pt) const {
		double sqrDist = pos.sqrDistance(pt);
		return (sqrDist <= sqr(getRadius()));
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

	constexpr bool containsPoint(const PointF& pt) const {
		return shape.containsPoint(pt);
	}
};

#endif // STAGEEDITOROBJECTS_HPP