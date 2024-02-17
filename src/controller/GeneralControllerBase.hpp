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
 * @details There are no methods that the descendants are forced to implement.
 *          They may choose to override `createReplacement()` if they are not
 *          "point of no return", or `on<eventName>` if they are willing to
 *          listen to that event.
 */
class GeneralControllerBase : public IControllerChild {
private:
	void replaceController();
	void pauseController();
protected:
	IControllerChild::IParent* parent;
	std::shared_ptr<ISysProxy> sysProxy;

	/**
	 * @brief Creates a replacement for this current controller.
	 * 
	 * @details Once the controller finishes it should replace itself in the
	 *          parent. This class already takes care of calling the replacement
	 *          method, descendants only need to provide the correct
	 *          replacement.
	 */
	virtual std::shared_ptr<IControllerChild> createReplacement();

	// The connections between begin/end events:
	//
	// 1) onStarted -+
	//               +---> 2) onActivated
	// 1) onResumed -+
	//
	//
	// 2) onFinished -+
	//                |
	// 2) onStopped --+
	//                +---> 1) onDeactivated
	// 2) onAborted --+
	//                |
	// 2) onPaused ---+
	//
	// Public `<eventName>Event` correspond to protected `on<eventName>`, so
	// when the public variant is called "from the outside", the call is
	// delegated to the protected variant. The exceptions are the begin/end
	// events; although their protected counterpart is called, there are other
	// events, which are called together with them. Which ones and their order
	// is shown in the graph above. For example, when `finishedEvent()` is
	// called, it calls `onDeactivated()`, then `onFinished()`.

	virtual void onActivated();
	virtual void onDeactivated();
	virtual void onStarted();
	virtual void onFinished();
	virtual void onStopped();
	virtual void onAborted();
	virtual void onPaused();
	virtual void onResumed();
	virtual void onKeyDown(KeyCode key);
	virtual void onTextInput(const char* text);
	virtual void onMouseBtnDown(MouseBtn btn, int x, int y);
	virtual void onMouseBtnUp(MouseBtn btn, int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseWheel(int dx, int dy);
	virtual void onLoop();
	virtual void onPaint(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
public:
	/**
	 * @brief Constructs a new GeneralControllerBase object.
	 * 
	 * @param sysProxy Proxy for communication with the system manager
	 *                 (dependency injection).
	 */
	GeneralControllerBase(std::shared_ptr<ISysProxy> sysProxy);

	/**
	 * @brief The controller has started.
	 * 
	 * @details The first event that the controller receives. Should only be
	 *          called by its parent.
	 */
	void startedEvent() override;
	/**
	 * @brief The controller has finished.
	 * 
	 * @details The last event that the controller receives. Should only be
	 *          called by the controller itself.
	 */
	void finishedEvent() override;
	/**
	 * @brief The controller has stopped.
	 * 
	 * @details The last event that the controller receives. Should only be
	 *          called by its parent.
	 */
	void stoppedEvent() override;
	/**
	 * @brief The controller has stopped.
	 * 
	 * @details The last event that the controller receives. Puts more emphasis
	 *          on the "exceptional termination". May be called both by its
	 *          parent and the controller itself.
	 */
	void abortedEvent() override;
	/**
	 * @brief The controller has stopped, but should not reset its state as it
	 *        will be resumed again.
	 * 
	 * @details Should only be called by the controller itself. Should call
	 *          The next event it receives should be either `resumedEvent()`
	 *          or any of the "end" events.
	 */
	void pausedEvent() override;
	/**
	 * @brief The controller was awoken after `pausedEvent()`.
	 * 
	 * @details Should only be called by its parent.
	 */
	void resumedEvent() override;

	/**
	 * @brief A key was pressed.
	 * 
	 * @details Only keys on the keyboard are observed. For mouse buttons use
	 *          mouseBtnDownEvent().
	 * 
	 * @param key The code of the key.
	 */
	void keyDownEvent(KeyCode key) override;
	/**
	 * @brief A character key was pressed.
	 * 
	 * @param text The null-terminated input text in UTF-8 encoding.
	 * 
	 * @remark The parameter type is compatible with `SDL_TextInputEvent`.
	 *         For more details, refer to:
	 *         https://wiki.libsdl.org/SDL2/SDL_TextInputEvent
	 * 
	 * @example ```
	 *          std::string s = "Input character: ";
	 *          s += text;
	 *          ```
	 */
	void textInputEvent(const char* text) override;
	/**
	 * @brief A mouse button was pressed.
	 * 
	 * @param btn The code of the button.
	 * @param x The mouse x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The mouse y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	void mouseBtnDownEvent(MouseBtn btn, int x, int y) override;
	/**
	 * @brief A mouse button was released.
	 * 
	 * @param btn The code of the button.
	 * @param x The mouse x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The mouse y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	void mouseBtnUpEvent(MouseBtn btn, int x, int y) override;
	/**
	 * @brief A mouse cursor moved.
	 * 
	 * @param x The x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	void mouseMoveEvent(int x, int y) override;
	/**
	 * @brief A mouse wheel has been scrolled.
	 * 
	 * @param dx The amount scrolled horizontally, positive to the right and
	 *           negative to the left.
	 * @param dy The amount scrolled vertically, positive away from the user
	 *           and negative towards the user.
	 */
	void mouseWheelEvent(int dx, int dy) override;
	/**
	 * @brief Event that happens every event loop iteration.
	 */
	void loopEvent() override;
	/**
	 * @brief Request to paint.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The recipent may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	void paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) override;
	/**
	 * @brief Assigns a parent of the controller.
	 * 
	 * @details Once a child finishes, it may (and sometimes must) provide
	 *          a replacement for itself.
	 * 
	 *          Modifies `parent`.
	 * 
	 *          Descendants should call this method if they override it.
	 */
	void setParent(IControllerChild::IParent* parent) override;
};

#endif // GENERALCONTROLLERBASE_HPP
