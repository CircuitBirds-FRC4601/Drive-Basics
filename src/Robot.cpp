#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <WPILib.h>
#include <LiveWindow/LiveWindow.h>
#include <LiveWindow/LiveWindow.h>
#include <WPILib.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <CameraServer.h>
#include <unistd.h>
#include <IterativeRobot.h>


class Robot: public frc::IterativeRobot {
private:
	//Auto Names
	
	void RobotInit() {
		//Auto Chooser
		chooser.AddDefault(autoForward,autoForward);
		chooser.AddDefault(autoForwardBox,autoForwardBox);
		chooser.AddObject(autoNone, autoNone);
		frc::SmartDashboard::PutData("Auto Modes",&chooser);
		//Auto Chooser

		//Vision Detachment
		std::thread visionThread(VisionThread);
		visionThread.detach();
		//Vision Detachment
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~AUTO START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void AutonomousInit() override{

		tilt->Set(frc::DoubleSolenoid::kReverse);//PUT THE GUN DOWN! Puts arms down

		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();//Reads the magic field config


		autoSelected = chooser.GetSelected();//Grab the chosen Auto
		std::cout << "Auto selected: " << autoSelected << std::endl;



		encLeft->Reset();//reset encoder values
		encRight->Reset();

		rDis=0;
		lDis=0;
		dumm=false;
		leg0=false;
		leg1=false;
		leg2=false;
	}

	void AutonomousPeriodic() {
		lDis=encLeft->GetRaw();//Grabs Encoder Values
		rDis=encRight->GetRaw();

		//Forward Simple Easy
		//if(autoSelected==autoForward){//10ft line + 1ft forward
			if(abs(rDis)<=132*encRes&&abs(lDis)<=132*encRes){
				lDrive=.7;
				rDrive=.7;
			}
			else{
				lDrive=0;
				rDrive=0;
			}
		//}
		//Forward Simple Easy

		// ****************************************Forward Box*****************************************************

	/*	else if(autoSelected==autoForwardBox){//Goes Forward and sees if the FMS says it is on our side;
			if(!leg0&&abs(rDis)<=168*encRes&&abs(lDis)<=168*encRes){
				lDrive=.7;
				rDrive=.7;
			}


			else if(!leg0){//Reset for SPIN!
				encLeft->Reset();
				encRight->Reset();
				leg0=true;//First leg finished on to 2;
			}



			else if(!leg1){//SPIN Check
				if(gameData.length()>0){//make sure there is data
					if(gameData[0]=='L'){//Is the Switch Left?
						if(abs(rDis)<=25*encRes&&abs(lDis)<=25*encRes){//SPIN! 90deg spin ~24.7in
							lDrive=.65;
							rDrive=-.65;
						}
						else{//PERISCOPE UP!
							lDrive=0;
							rDrive=0;
							elevator->Set(.75);
							sleep(1);
							elevator->Set((3.0/8.0));
							encLeft->Reset();
							encRight->Reset();
							leg1=true;//We Done Spinning!

						}
					}

					else{//It is not on the Left STOP!
						leg2=1;
						leg1=1;
					}
				}

				else{//NO DATA AHHHHHHHHHHHHHHHHH ERRRRRRROOOOOORRR!!!!!
					leg2=1;
					leg1=1;
				}

			}


			else if(!leg2){//Final distance
				if(abs(rDis)<=50*encRes&&abs(lDis)<=50*encRes){//55.56inch some wiggly room so we dont slam into it
					lDrive=.7;
					rDrive=.7;
				}
				else{
					shooter->Set(1);
					sleep(2);//give the box time to get out of there
					arm->Set(frc::DoubleSolenoid::kReverse);//DROP DA BOMB!!!!
					shooter->Set(0);
					leg2=true;
				}
			}

			else{// *Sigh* and now we are done
				lDrive=0;
				rDrive=0;
			}
		}
		// ****************************************Forward Box*****************************************************
		else {
			lDrive=0;
			rDrive=0;
		}*/
		//Print Encoder Values
		SmartDashboard::PutNumber("Right Encoder", rDis);
		SmartDashboard::PutNumber("Right Encoder", lDis);
		//Print Encoder Values

		robotDrive->TankDrive(lDrive,rDrive);//Drives based on previous Drive values
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~AUTO END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TELE START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void TeleopInt() {
		garry->Enabled();//turn the compressor on
		dumm=0;
	}

	void TeleopPeriodic() {
		//DRIVE

		lDrive=-.85*leftStick->GetRawAxis(1);
		rDrive=-.85*rightStick->GetRawAxis(1);
		robotDrive->TankDrive(lDrive,rDrive);

		//DRIVE END

		//**********************************Start Elevation*******************************************************************

		elevation = gamePad->GetRawAxis(1);
		if (fabs(elevation) < .1) {//little dead zone for the sticks
			elevation = 0;
		}

		if(!dumm) {
			elevator->Set(.75*elevation);//The Motors lifting the stages.
		}
		else{
			elevator->Set(0);
		}

		climby=gamePad->GetRawButton(8);
		if(climby){
			climber->Set(1);//The motor for lifting the robot
			dumm=1;
		}
		else{
			climber->Set(0);
			dumm=0;

		}
		//**********************************End Elevation*******************************************************************


		//**********************************Start Box Grabber*******************************************************************

		//Arm
		armout=gamePad->GetRawButton(1);
		armin=gamePad->GetRawButton(2);

		if(armout){
			arm->Set(frc::DoubleSolenoid::kReverse);
		}
		else if(armin){
			arm->Set(frc::DoubleSolenoid::kForward);
		}
		//Arm End

		//Shooter
		shotIn=gamePad->GetRawAxis(2);
		shotOut=gamePad->GetRawAxis(3);

		if(shotIn){
			shooter->Set(-1);
		}
		else if(shotOut){
			shooter->Set(1);
		}
		else{
			shooter->Set(0);
		}
		//Shooter END

		//Tilter

		if(gamePad->GetRawButton(7)&&!tiltdum){//tilt the arms up and down with a dummy value so you don't double press
			tilter=!tilter;
		}
		if(tilter){
			tilt->Set(frc::DoubleSolenoid::kForward);
		}
		else{
			tilt->Set(frc::DoubleSolenoid::kReverse);
		}
		tiltdum=gamePad->GetRawButton(7);

		//Tilter END

		//**********************************End Box Grabber*******************************************************************

		SmartDashboard::PutNumber("Left",encLeft->GetRaw());//Use the smart dashboard to display information
		SmartDashboard::PutNumber("Right", encRight->GetRaw());

	}
	void TestPeriodic() {

	}

};

START_ROBOT_CLASS(Robot)

/* Hardware map of the robot "TBA"  (CB5)
 *	1in= ~56 Wheel Encoders
 *		Game Pad
 *		Axis
 *		0 LX) Elevator
 *		1 LY)
 *		2 LTrig) Shooter In
 *		3 RTrig) Shooter Out
 *		4 RX)
 *		5 RY)
 *
 *		Button
 *		1 A) Arm Out
 *		2 B) Arm In
 *		3 X)
 *		4 Y)
 *		5 LB)
 *		6 RB)
 *		7 BCK) Tilt
 *		8 STR) Climb
 *		9 LSTK)
 *		10 RSTK)
 *
 *		RRio Pins
 * 		PWM
 *		0 Shooter
 *		1 Angler
 *		2 Front	Right
 *		3 Back	"
 *		4 Climber
 *		5 Elevator
 *		6 Front Left
 *		7 Back	"
 *		8
 *		9
 *
 *
 *  	DIO
 *  	0 Left Encoder  A
 *  	1 	   "		B
 *  	2 Right Encoder A
 *  	3	   "		B
 *  	4 	Climber     A
 *  	5 	   "	 	B
 *  	6
 *  	7
 *  	8
 *  	9
 *  	10
 *
 *
 *  	Analog
 *  	0
 *  	1
 *  	2
 *  	3
 *
 *		Relay
 *		0
 *		1
 *		2
 *		3
 *
 *
 *
 *
 *
 */
