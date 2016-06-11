#include "LightController.h"


namespace fstoolbox
{
	bool LightController::lights_brightness = true; //1: bright, 0: dimmed
	bool LightController::lights_power = true; //1: power on, 0: power off
	bool LightController::lights_test = false; //0: normal, : all lights on
	bool LightController::is_debug = true;
	bool LightController::updateLocked = true;
	std::map<FSIID, LightControllerLight> LightController::lightsList;


	LightController::LightController()
	{

		FSIcm.inst.OnVarReceiveEvent += fsiOnVarReceive;
		FSIcm.inst.DeclareAsWanted(new FSIID[]
		{
			FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_DIM_POS,
			FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_TEST_POS
		}
		);

		FSIcm.inst.ProcessWrites();
	}


	LightController::~LightController()
	{
	}




	void LightController::fsiOnVarReceive(FSIID id)
	{

		if (id == FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_DIM_POS || id == FSIID::MBI_MIP_CM1_LIGHTS_TEST_SWITCH_TEST_POS)
		{
			if (FSIcm.inst.MBI_MIP_CM1_LIGHTS_TEST_SWITCH_DIM_POS)
			{
				debug("MIP Lights Dim");
				setLightBrightness(false);
				setLightTest(false);
			}
			else if (FSIcm.inst.MBI_MIP_CM1_LIGHTS_TEST_SWITCH_TEST_POS)
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
		if (!lightsList.ContainsKey(id))
		{
			String name = id.ToString("g") + "*";

			Type type = FSIcm.inst.GetType();
			MemberInfo[] memberInfos = type.GetMember(name);

			if (memberInfos.Length == 1)
			{
				lightsList.Add(id, new LightControllerLight(memberInfos[0]));
			}
			else if (memberInfos.Length == 2)
			{
				lightsList.Add(id, new LightControllerLight(memberInfos[0], memberInfos[1]));
			}
			else
			{
				Console.WriteLine("ERROR: LightController: Too many Lights found for name: " + name);
				return;
			}
		}

		//set the light value
		lightsList[id].set(value);
		lightsList[id].writeStatus(lights_power, lights_test, lights_brightness, ref FSIcm.inst);
	}



	void LightController::ProcessWrites()
	{
		FSIcm.inst.ProcessWrites();
	}



	void LightController::updateAll()
	{
		if (!updateLocked)
			//enure that this is not called until all lights have been added to the lightsList
		{
			for (int i = 0; i < lightsList.Count; i++)
			{
				//Variable nicht Thread-Safe! Bei Errors erstmal auf weiter drücken
				lightsList.ElementAt(i).Value.writeStatus(lights_power, lights_test, lights_brightness, ref FSIcm.inst);
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