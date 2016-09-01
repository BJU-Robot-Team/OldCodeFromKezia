#ifndef MOTOR_CONTROLLER
#define MOTOR_CONTROLLER


class MotorController
{

public:

	MotorController();
	~MotorController();


	int getPortNum() { return m_portNum; }

	void setFSpeed(int speed);
	void setTSpeed(int speed);

	void sendMotorCommand();
private:
	int m_portNum;
	int m_lSpeed;
	int m_rSpeed;
};


#endif
