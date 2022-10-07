#include "robot.hpp"

Robot::Robot(/* args */)
{
}

Robot::~Robot()
{
}

void Robot::Init()
{
}

void Robot::Deinit()
{
}

void Robot::Reboot()
{
  HAL_NVIC_SystemReset();
}

bool Robot::IsMoving()
{
  return jointsStateFlag != 0b1111110;
}

void Robot::SetCommandMode(uint8_t _mode)
{
  commandMode = static_cast<CommandMode>(_mode);
  switch (commandMode)
  {
    case WAKEUP_MODE:
      Init();
      commandMode = COMMAND_CONTROL_MODE;
      break;
    case COMMAND_CONTROL_MODE:
      break;
    case SLEEP_MODE:
      Deinit();
      break;
    default:
      break;
  }
}

void Robot::Update()
{
  commandHandler.CheckLastCmdUpdate();
}

uint32_t Robot::CommandHandler::Push(const std::string& _cmd)
{
  osStatus_t status = osMessageQueuePut(commandFifo, _cmd.c_str(), 0U, 0U);
  if (status == osOK)
    return osMessageQueueGetSpace(commandFifo);

  return 0xFF;  // failed
}

void Robot::CommandHandler::EmergencyStop()
{
  context->isStopped = true;
  ClearFifo();
}

void Robot::CommandHandler::Resume()
{
  context->isStopped = false;
}

std::string Robot::CommandHandler::Pop(uint32_t timeout)
{
  osStatus_t status = osMessageQueueGet(commandFifo, strBuffer, nullptr, timeout);

  return std::string{ strBuffer };
}

uint32_t Robot::CommandHandler::GetSpace()
{
  return osMessageQueueGetSpace(commandFifo);
}

void Robot::CommandHandler::CheckLastCmdUpdate()
{
  if ((millis() - context->cmd.last_cmd_update) > (context->RATETOMILLIS / context->thresh_last_cmd_update_rate))
  {
    // cmd input state
    context->commandInputState = NO_COMMAND_INPUTTED;
  }
}

uint32_t Robot::CommandHandler::ParseCommand(const std::string& _cmd)
{
  uint8_t argNum;
  char buff1[10] = {};

  switch (context->commandMode)
  {
    case WAKEUP_MODE:
      break;
    case COMMAND_CONTROL_MODE:
      if (_cmd[0] == '>')
      {
        // you should set the command msg to context->cmd & context->commandInputState
        // // Check data quantity
        // auto cnt_comma = (uint8_t)std::count(std::cbegin(_cmd), std::cend(_cmd), ',');
        // auto cnt_dot = (uint8_t)std::count(std::cbegin(_cmd), std::cend(_cmd), '.');
        // if ((cnt_comma == 5) && (cnt_dot == 6))
        // {
        //   context->cmd.last_cmd_update = millis();
        //   context->commandInputState = VELOCITY_COMMAND_INPUTTED;
        //   float velocity[6] = {};
        //   argNum = sscanf(_cmd.c_str(), "%c%f,%f,%f,%f,%f,%f", buff1, velocity, velocity + 1, velocity + 2,
        //                   velocity + 3, velocity + 4, velocity + 5);
        //   //! ROS Twist msg[6] -> vector[3]
        //   context->cmd.velocity[0] = velocity[0];
        //   context->cmd.velocity[1] = velocity[1];
        //   context->cmd.velocity[2] = velocity[5];
        return 1;
      }
      else if (_cmd[0] == '@')
      {
        return 1;
      }
      break;
    case SLEEP_MODE:
      break;
  }

  return osMessageQueueGetSpace(commandFifo);
}

void Robot::CommandHandler::ClearFifo()
{
  osMessageQueueReset(commandFifo);
}
