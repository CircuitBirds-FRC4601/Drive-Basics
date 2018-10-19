#include <iostream>
#include <memory>
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
public:
	double lDrive=0,rDrive=0;
	Spark *fRight =new Spark(2);
	Spark *bRight =new Spark(3);
	Spark *climber =new Spark(4);
	Spark *fLeft =new Spark(6);
	Spark *bLeft =new Spark(7);
	
	Joystick *leftStick =new Joystick(0);
	Joystick *rightStick =new Joystick(1);
	Joystick *gamePad =new Joystick(2);



	frc::RobotDrive *robotDrive =new frc::RobotDrive (fLeft,bLeft,fRight,bRight);

	
	
	void RobotInit() {
		
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~AUTO START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void AutonomousInit() override{

	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~AUTO END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TELE START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void TeleopInt() {
		
	}

	void TeleopPeriodic() {
		//DRIVE

		lDrive=-.85*leftStick->GetRawAxis(1);
		rDrive=-.85*rightStick->GetRawAxis(1);
		robotDrive->TankDrive(lDrive,rDrive);

		//DRIVE END


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
