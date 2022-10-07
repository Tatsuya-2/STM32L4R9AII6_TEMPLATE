#ifndef STM32_FW_ROBOT_H
#define STM32_FW_ROBOT_H

#include "common_inc.h"
#include <algorithm>
#include <vector>

class Robot
{
private:
  struct RobotCmd
  {
    // std::vector<float> velocity{ 0.0, 0.0, 0.0 }; // example
    uint32_t last_cmd_update = 0;
  };
  u_int32_t thresh_last_cmd_update_rate = 1;  // Hz
  const u_int32_t RATETOMILLIS = 1000;

public:
  explicit Robot(/* args */);
  ~Robot();

  enum CommandMode
  {
    WAKEUP_MODE = 1,
    COMMAND_CONTROL_MODE,
    SLEEP_MODE
  };

  enum CommandInputState
  {
    NO_COMMAND_INPUTTED = 1,
    // VELOCITY_COMMAND_INPUTTED // example
  };

  void Init();
  void Deinit();
  bool IsMoving();
  void Reboot();
  void SetCommandMode(uint8_t _mode);
  void Update();

  //! Communication protocol definitions
  auto MakeProtocolDefinitions()
  {
    return make_protocol_member_list(make_protocol_function("reboot", *this, &Robot::Reboot),
                                     make_protocol_function("set_command_mode", *this, &Robot::SetCommandMode, "mode"));
  }

  volatile uint8_t jointsStateFlag = 0b00000000;
  CommandMode commandMode = COMMAND_CONTROL_MODE;
  CommandInputState commandInputState = NO_COMMAND_INPUTTED;
  bool isStopped = false;

  RobotCmd cmd;

  class CommandHandler
  {
  public:
    explicit CommandHandler(Robot* _context) : context(_context)
    {
      commandFifo = osMessageQueueNew(16, 64, nullptr);
      commandLifo = osMessageQueueNew(1, 64, nullptr);
    }

    uint32_t Push(const std::string& _cmd);
    std::string Pop(uint32_t timeout);
    uint32_t ParseCommand(const std::string& _cmd);
    uint32_t GetSpace();
    void CheckLastCmdUpdate();
    void ClearFifo();
    void EmergencyStop();
    void Resume();

  private:
    Robot* context;
    osMessageQueueId_t commandFifo;
    osMessageQueueId_t commandLifo;
    char strBuffer[64]{};
  };
  CommandHandler commandHandler = CommandHandler(this);
};

#endif  // STM32_FW_ROBOT_H
