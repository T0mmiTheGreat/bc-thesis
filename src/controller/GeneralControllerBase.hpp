/**
 * @file GeneralControllerBase.hpp
 * @author Tomáš Ludrovan
 * @brief GeneralControllerBase class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GENERALCONTROLLERBASE_HPP
#define GENERALCONTROLLERBASE_HPP

#include <memory>

#include "types.hpp"
#include "canvas/ICanvas.hpp"
#include "controller/IControllerChild.hpp"
#include "sysproxy/ISysProxy.hpp"

/**
 * @brief The base class for general (non-root) controllers.
 * 
 * @details Provides convenient overrides for events, so if descendants don't
 *          need to listen to some events, they don't need to explicitly
 *          override the pure virtual methods and make their body empty.
 * 
 *          There are no methods that the descendants are forced to implement.
 *          They may choose to override `createReplacement()` if they are not
 *          "point of no return". They MUST call this class's constructor if
 *          they provide their own.
 */
class GeneralControllerBase : public IControllerChild {
protected:
	SwapCallback swapCallback;
	std::shared_ptr<ISysProxy> sysProxy;

	/**
	 * @brief Creates the replacement for this current controller.
	 * 
	 * @details Once the controller finishes it should call the swap callback
	 *          to replace itself in the parent. This class already takes care
	 *          of calling the callback, descendants only need to provide the
	 *          correct replacement.
	 */
	virtual std::unique_ptr<IControllerChild> createReplacement();
public:
	/**
	 * @brief Constructs a new GeneralControllerBase object.
	 * 
	 * @param sysProxy Proxy for communication with the system manager
	 *                 (dependency injection).
	 */
	GeneralControllerBase(std::shared_ptr<ISysProxy> sysProxy);
	/**
	 * @brief The controller should start.
	 * 
	 * @details The first event that the controller receives.
	 */
	virtual void startedEvent() override;
	/**
	 * @brief The controller should end.
	 * 
	 * @details The last event that the controller receives. Should be sent
	 *          only by the controller itself and not its parent.
	 * 
	 *          Automatically calls the swap callback.
	 * 
	 *          Descendants should call this method if they override it.
	 */
	virtual void finishedEvent() override;
	/**
	 * @brief The controller should stop.
	 * 
	 * @details In most cases identical to finishedEvent(). Should only be sent
	 *          by its parent and not the controller itself.
	 * 
	 *          Delegated to finishedEvent.
	 * 
	 *          Descendants should call this method if they override it.
	 */
	virtual void stoppedEvent() override;
	/**
	 * @brief The controller should stop.
	 * 
	 * @details In most cases identical to finishedEvent(). May be sent both
	 *          by parent and the controller itself. Puts more emphasis on the
	 *          "exceptional termination".
	 * 
	 *          Delegated to finishedEvent.
	 * 
	 *          Descendants should call this method if they override it.
	 */
	virtual void abortedEvent() override;
	/**
	 * @brief A key was pressed.
	 * 
	 * @details Only keys on the keyboard are observed. For mouse buttons use
	 *          mouseBtnDownEvent().
	 * 
	 * @param key The code of the key.
	 */
	virtual void keyDownEvent(KeyCode key) override;
	/**
	 * @brief A mouse button was pressed.
	 * 
	 * @param btn The code of the button.
	 * @param x The mouse x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The mouse y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	virtual void mouseBtnDownEvent(MouseBtn btn, int x, int y) override;
	/**
	 * @brief A mouse button was released.
	 * 
	 * @param btn The code of the button.
	 * @param x The mouse x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The mouse y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	virtual void mouseBtnUpEvent(MouseBtn btn, int x, int y) override;
	/**
	 * @brief A mouse cursor moved.
	 * 
	 * @param x The x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	virtual void mouseMoveEvent(int x, int y) override;
	/**
	 * @brief A mouse wheel has been scrolled.
	 * 
	 * @param dx The amount scrolled horizontally, positive to the right and
	 *           negative to the left.
	 * @param dy The amount scrolled vertically, positive away from the user
	 *           and negative towards the user.
	 */
	virtual void mouseWheelEvent(int dx, int dy) override;
	/**
	 * @brief Event that happens every event loop iteration.
	 */
	virtual void loopEvent() override;
	/**
	 * @brief Request to paint.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The recipent may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	virtual void paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) override;
	/**
	 * @brief Assigns function for swapping the child with a new child.
	 * 
	 * @details Once a child finishes, it may (and sometimes must) provide
	 *          a replacement for itself. To avoid circular references the
	 *          child must not keep a pointer to its parent, so instead it
	 *          receives a function pointer from its parent which will replace
	 *          the child with the provided controller.
	 * 
	 *          Modifies `swapCallback`.
	 * 
	 *          Descendants should call this method if they override it.
	 */
	virtual void setSwapCallback(SwapCallback f) override;
};

#endif // GENERALCONTROLLERBASE_HPP
