/**
 * @file SysEventPublisherFactory.hpp
 * @author Tomáš Ludrovan
 * @brief SysEventPublisherFactory class
 * @version 0.1
 * @date 2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SYSEVENTPUBLISHERFACTORY_HPP
#define SYSEVENTPUBLISHERFACTORY_HPP

#include <memory>

#include "ISysEventPublisher.hpp"

/**
 * @brief Class for constructing the default implementation of ISysEventPublisher.
 * 
 */
class SysEventPublisherFactory {
public:
	/**
	 * @brief Constructs the default implementation of ISysEventPublisher.
	 */
	std::shared_ptr<ISysEventPublisher> createPublisher() const;
};


#endif // SYSEVENTPUBLISHERFACTORY_HPP
