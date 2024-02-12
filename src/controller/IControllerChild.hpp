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
	class IParent {
	public:
		virtual ~IParent() {}
		/**
		 * @brief Replaces the current child with `replacement`.
		 */
		virtual void replaceController(
			std::shared_ptr<IControllerChild> replacement) = 0;
		/**
		 * @brief Replaces the current child with `replacement`, but keeps the
		 *        current child alive in a stack structure.
		 * 
		 * @details Once the `replacement` finishes, if it does not provide a
		 *          replacement itself (replaces with `nullptr`), the child at
		 *          the top of the stack should be awoken again.
		 */
		virtual void pauseController(
			std::shared_ptr<IControllerChild> replacement) = 0;
	};
public:
	virtual ~IControllerChild() {}
	/**
	 * @brief Assigns a parent of the controller.
	 * 
	 * @details Once a child finishes, it may (and sometimes must) provide
	 *          a replacement for itself.
	 */
	virtual void setParent(IControllerChild::IParent* parent) = 0;
};

#endif // ICONTROLLERCHILD_HPP
