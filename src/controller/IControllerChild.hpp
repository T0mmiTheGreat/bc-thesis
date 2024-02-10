/**
 * @file IControllerChild.hpp
 * @author Tomáš Ludrovan
 * @brief IControllerChild interface
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ICONTROLLERCHILD_HPP
#define ICONTROLLERCHILD_HPP

#include <functional>
#include <memory>

#include "controller/IControllerEventSubscriber.hpp"

class IControllerChild;

typedef std::function<void(std::shared_ptr<IControllerChild>)> SwapCallback;

/**
 * @brief Controller that may be child for another controller.
 */
class IControllerChild : public IControllerEventSubscriber {
public:
	virtual ~IControllerChild() {}
	/**
	 * @brief Assigns function for swapping the child with a new child.
	 * 
	 * @details Once a child finishes, it may (and sometimes must) provide
	 *          a replacement for itself. To avoid circular references the
	 *          child must not keep a pointer to its parent, so instead it
	 *          receives a function pointer from its parent which will replace
	 *          the child with the provided controller.
	 */
	virtual void setSwapCallback(SwapCallback f) = 0;
};

#endif // ICONTROLLERCHILD_HPP
