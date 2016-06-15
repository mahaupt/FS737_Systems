#pragma once
#include <map>


#include "FSInterface/FSIID.h"
#include "FSIcm.h"

/*
*
* This class controls the control lights.
* If there is no power, this class turns all lights to off, or to on for testing
* You can also set light failures here
*
*/
namespace fstoolbox
{
	using fsinterface::FSIID;


	class LightControllerLight
	{
	private:
		int light_status = 1; //0: always_off, 1: normal, 2: always_on
		bool light_on = false;
		bool is_dimmable = false;

		FSIID light, dimmed_light;

	public:
		LightControllerLight() {}
		LightControllerLight(FSIID _light, FSIID _dimmable_light);
		LightControllerLight(FSIID _light);
        
		void set(bool value);
		bool get(bool main_light_power, bool lights_test);
		void writeStatus(bool main_lights_power, bool lights_test, bool light_brightness);
        void setOverride(int status); //see light_status
	};




	typedef std::map<FSIID, LightControllerLight> LightsList;

	class LightController
	{
	private:
		static bool lights_brightness; //1: bright, 0: dimmed
		static bool lights_power; //1: power on, 0: power off
		static bool lights_test; //0: normal, : all lights on
		static bool is_debug;
		static bool updateLocked;

		static LightsList lightsList;
		static void updateAll();
	public:
		LightController();
		~LightController();

		static void fsiOnVarReceive(FSIID id);
		static void set(FSIID id, bool value);
        static void setOverride(FSIID id, int value);
		static void registerLight(FSIID light, FSIID dimmed_light = (FSIID)0);
		static void ProcessWrites();
		static void setLightPower(bool _light_power);
		static void setLightTest(bool _light_test);
		static void setLightBrightness(bool _lights_brightness);
		static void debug(std::string str);
		static void enableUpdate();
	};

}
