/**
 * @file StageSerializerFactory.hpp
 * @author Tomáš Ludrovan
 * @brief StageSerializerFactory class
 * @version 0.1
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGESERIALIZERFACTORY_HPP
#define STAGESERIALIZERFACTORY_HPP

#include <memory>

#include "stageserializer/IStageSerializer.hpp"

class StageSerializerFactory {
public:
	static std::shared_ptr<IStageSerializer> createDefault();
	static std::shared_ptr<IStageSerializer> createYAMLStageSerializer();
};

#endif // STAGESERIALIZERFACTORY_HPP
