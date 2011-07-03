/**
 * @file:      GO_IControllable.h
 * @author:    Nick Appleton
 *
 *
 */

#ifndef GO_ICONTROL_H_
#define GO_ICONTROL_H_

#include "GO_IRenderable.h"

class GO_IControl : public GO_IRenderable
{

public:

	typedef enum
	{
		GO_MB_LEFT,
		GO_MB_RIGHT
	} GO_MouseButton;

	virtual ~GO_IControl() { };

	/**
	 * Method to be called when control is clicked.
	 */
	virtual void MouseButtonDown(const unsigned x, const unsigned y, const GO_MouseButton button) { };

	/**
	 * Method to be called when control scrolled.
	 */
	virtual void Scroll(const unsigned x, const unsigned y, const int amount) { };

};

#endif /* GO_ICONTROL_H_ */
