#include "LightController.h"


namespace fstoolbox
{

	LightControllerLight::LightControllerLight(FSIID _light, FSIID _dimmable_light)
	{
		light = _light;
		dimmed_light = _dimmable_light;
		is_dimmable = true;
	}

	LightControllerLight::LightControllerLight(FSIID _light)
	{
		light = _light;
		is_dimmable = false;
	}


	//set the light status
	void LightControllerLight::set(bool value)
	{
		light_on = value;
	}

	//get if the light is on or off
	bool LightControllerLight::get(bool main_light_power, bool lights_test)
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


	void LightControllerLight::writeStatus(bool main_lights_power, bool lights_test, bool light_brightness)
	{
		bool true_light_status = get(main_lights_power, lights_test);

		//write light status
		if (!is_dimmable)
		{
			FSIcm::inst->set<bool>(light, true_light_status);
		}
		else
		{
			//bright
			FSIcm::inst->set<bool>(light, true_light_status);

			//dimmed
			FSIcm::inst->set<bool>(dimmed_light, !light_brightness);
		}


	}
    
    
    void LightControllerLight::setOverride(int status) {
        light_status = status;
    }






	bool LightController::lights_brightness = true; //1: bright, 0: dimmed
	bool LightController::lights_power = true; //1: power on, 0: power off
	bool LightController::lights_test = false; //0: normal, : all lights on
	bool LightController::is_debug = true;
	bool LightController::updateLocked = true;
	LightsList LightController::lightsList;


	LightController::LightController()
	{

		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_DIM_POS,
			FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_TEST_POS
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		FSIcm::inst->ProcessWrites();
	}


	LightController::~LightController()
	{
	}




	void LightController::fsiOnVarReceive(FSIID id)
	{

		if (id == FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_DIM_POS || id == FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_TEST_POS)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_DIM_POS))
			{
				debug("MIP Lights Dim");
				setLightBrightness(false);
				setLightTest(false);
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_TEST_POS))
			{
				debug("MIP Lights Test");
				setLightBrightness(true);
				setLightTest(true);
			}
			else
			{

				debug("MIP Lights Norm");
				setLightTest(false);
				setLightBrightness(true);
			}
			ProcessWrites();
		}
	}




	void LightController::set(FSIID id, bool value)
	{
		//no id in lights list
		if (lightsList.find(id) == lightsList.end())
		{
            std::cerr << "Light id:" << std::to_string((int)id) << " not registered! Aborted" << std::endl;
			return;
		}

		//set the light value
		lightsList[id].set(value);
		lightsList[id].writeStatus(lights_power, lights_test, lights_brightness);
	}
    
    ///<summary>
    /// This Overrides the Light status to set a light failure or a private light test
    ///</summary>
    void LightController::setOverride(FSIID id, int value) {
        //no id in lights list
        if (lightsList.find(id) == lightsList.end())
        {
            std::cerr << "Light id:" << std::to_string((int)id) << " not registered! Aborted" << std::endl;
            return;
        }
        
        //set the light value
        lightsList[id].setOverride(value);
        lightsList[id].writeStatus(lights_power, lights_test, lights_brightness);
    }

	///<summary>
	/// This registeres a light in the LightController.
	/// If you wish to control a light with the lightcontroller, the light has to be registered first
	/// If the light is dimmable, set the dimmable FSIID as the second parameter.
	/// The class will set the dimmed light automatically
	///</summary>
	void LightController::registerLight(FSIID light, FSIID dimmed_light) {
		//id in lights list
		if (lightsList.find(light) != lightsList.end())
		{
            std::cerr << "Light id:" << std::to_string((int)light) << " already registered! Aborted" << std::endl;
			return;
		}

		//add light to lights list
		if (dimmed_light == (FSIID)0) {
			lightsList.emplace(light, LightControllerLight(light));
		} else {
			lightsList.emplace(light, LightControllerLight(light, dimmed_light));
		}

		set(light, false);
	}



	void LightController::ProcessWrites()
	{
		FSIcm::inst->ProcessWrites();
	}



	void LightController::updateAll()
	{
		if (!updateLocked)
			//enure that this is not called until all lights have been added to the lightsList
		{
			for (LightsList::iterator it = lightsList.begin(); it != lightsList.end(); it++)
			{
				//Variable nicht Thread-Safe! Bei Errors erstmal auf weiter drücken
				it->second.writeStatus(lights_power, lights_test, lights_brightness);
			}
		}
	}


	void LightController::setLightPower(bool _light_power)
	{
		lights_power = _light_power;
		updateAll();
	}


	void LightController::setLightTest(bool _light_test)
	{
		lights_test = _light_test;
		updateAll();
	}


	void LightController::setLightBrightness(bool _lights_brightness)
	{
		lights_brightness = _lights_brightness;
		updateAll();
	}


	void LightController::debug(std::string str)
	{
		if (is_debug)
		{
			std::cout << str << std::endl;
		}
	}


	void LightController::enableUpdate()
	{
		updateLocked = false;
		updateAll();
	}

}