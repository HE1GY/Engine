#include "Application.h"

#include <iostream>


#include "EventSys/KeyEvent.h"
#include "EventSys/ApplicationEvent.h"
#include "EventSys/MouseEvent.h"

#include "LogLib/LogLib.h"

namespace Engine
{
	void Application::Run()
	{
		EventSys::KeyPress keyPress(92,1);
		HZ_CORE_ERROR(keyPress.ToString());

		EventSys::KeyReleased keyr(92);
		HZ_CORE_ERROR(keyr.ToString());

		EventSys::KeyTyped keyt(91);
		HZ_CORE_ERROR(keyt.ToString());

		EventSys::WindowResized WindowResized(100,100);
		HZ_CORE_ERROR(WindowResized.ToString());

		EventSys::MouseMoved MouseMoved(96,1);
		HZ_CORE_ERROR(MouseMoved.ToString());

		EventSys::WindowClosed a;
		HZ_CORE_ERROR(a.ToString());

		EventSys::MouseButtonPressed MouseButtonPressed(50);
		HZ_CORE_ERROR(MouseButtonPressed.ToString());

		EventSys::MouseButtonReleased MouseButtonReleased(51);
		HZ_CORE_ERROR(MouseButtonReleased.ToString());

		EventSys::MouseScrolled MouseScrolled(96, 1);
		HZ_CORE_ERROR(MouseScrolled.ToString());

		while(true)
		{
			
		}
	}
}