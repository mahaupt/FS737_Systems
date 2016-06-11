#pragma once
#include <map>


#include "FSInterface/FSIClient.h"

/*
*
* This class controls the control lights.
* If there is no power, this class turns all lights to off, or to on for testing
* You can also set light failures here
*
*/
namespace fstoolbox
{

	typedef fsinterface::FSIID FSIID;

	class LightController
	{
	private:
		static bool lights_brightness; //1: bright, 0: dimmed
		static bool lights_power; //1: power on, 0: power off
		static bool lights_test; //0: normal, : all lights on
		static bool is_debug;
		static bool updateLocked;

		static std::map<FSIID, LightControllerLight> lightsList;
		static void updateAll();
	public:
		LightController();
		~LightController();

		static void fsiOnVarReceive(FSIID id);
		static void registerLight(FSIID light, FSIID dimmed_light = (FSIID)NULL);
		static void set(FSIID id, bool value);
		static void ProcessWrites();
		static void setLightPower(bool _light_power);
		static void setLightTest(bool _light_test);
		static void setLightBrightness(bool _lights_brightness);
		static void debug(std::string str);
		static void enableUpdate();
	};


	class LightControllerLight
	{
	private:
		int light_status = 1; //0: always_off, 1: normal, 2: always_on
		bool light_on = false;
		bool is_dimmable = false;

		FSIID lightMember, dimmedLightMember;

	public:
		LightControllerLight(FSIID _light, FSIID _dimmable_light)
		{
			lightMember = _light;
			dimmedLightMember = _dimmable_light;
			is_dimmable = true;
		}

		LightControllerLight(FSIID _light)
		{
			lightMember = _light;
			is_dimmable = false;
		}


		//set the light status
		void set(bool value)
		{
			light_on = value;
		}

		//get if the light is on or off
		bool get(bool main_light_power, bool lights_test)
		{
			if (main_light_power)
			{
				switch (light_status)
				{
				case (0):
				default:
					return false;
				case (1):
					if (lights_test)
						return true;
					return light_on;
				case (2):
					return true;
				}
			}
			else
			{
				return false;
			}

		}


		void writeStatus(bool main_lights_power, bool lights_test, bool light_brightness)
		{
			bool true_light_status = get(main_lights_power, lights_test);

			//write light status
			if (!is_dimmable)
			{
				//only bright lamp
				if (true_light_status != (bool)((PropertyInfo)lightMember).GetValue(fsi_object))
				{
					((PropertyInfo)lightMember).SetValue(fsi_object, true_light_status);
				}
			}
			else
			{
				//bright
				if (true_light_status != (bool)((PropertyInfo)lightMember).GetValue(fsi_object))
				{
					((PropertyInfo)lightMember).SetValue(fsi_object, true_light_status);
				}

				//dimmed
				if (!light_brightness != (bool)((PropertyInfo)dimmedLightMember).GetValue(fsi_object))
				{
					((PropertyInfo)dimmedLightMember).SetValue(fsi_object, !light_brightness);
				}
			}


		}
	};
}
