/**
 * @file StageSerializerFactory.cpp
 * @author Tomáš Ludrovan
 * @brief StageSerializerFactory class
 * @version 0.1
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageserializer/StageSerializerFactory.hpp"

#include "stageserializer/YAMLStageSerializer.hpp"

std::shared_ptr<IStageSerializer> StageSerializerFactory::createDefault()
{
	return createYAMLStageSerializer();
}

std::shared_ptr<IStageSerializer> StageSerializerFactory::createYAMLStageSerializer()
{
	return std::make_shared<YAMLStageSerializer>();
}
