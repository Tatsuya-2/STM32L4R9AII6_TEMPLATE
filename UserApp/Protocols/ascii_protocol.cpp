#include "common_inc.h"

extern Robot robot;

void CommonAsciiCmd(const char* _cmd, StreamSink& _responseChannel)
{
  std::string s(_cmd);
  if (s.find("STOP") != std::string::npos)
  {
    robot.commandHandler.EmergencyStop();
    Respond(_responseChannel, "!!!Stopped!!!");
  }
  else if (s.find("RESUME") != std::string::npos)
  {
    robot.commandHandler.Resume();
    Respond(_responseChannel, "Resumed");
  }
  else if (s.find("SYSTEMRESET") != std::string::npos)
  {
    robot.Reboot();
  }
  else if (s.find("KILLALL") != std::string::npos)
  {
    printf("Kill all\n");
  }
  else if (s.find("WAKEALL") != std::string::npos)
  {
    printf("Wake all\n");
  }
}

void ControlCmd(const char* _cmd)
{
  robot.commandHandler.Push(_cmd);
}

void CommandModeCmd(const char* _cmd)
{
  std::string s(_cmd);
  if (s.find("CMDMODE") != std::string::npos)
  {
    int mode = 0;
    sscanf(_cmd, "#CMDMODE%d", &mode);
    Robot::CommandMode commandMode_ = static_cast<Robot::CommandMode>(mode);
    robot.SetCommandMode(commandMode_);
  }
}

void OnUsbAsciiCmd(const char* _cmd, size_t _len, StreamSink& _responseChannel)
{
  /*---------------------------- ↓ Add Your CMDs Here ↓ -----------------------------*/
  if (_cmd[0] == '!')
  {
    CommonAsciiCmd(_cmd, _responseChannel);
  }
  else if (_cmd[0] == '#')
  {
    CommandModeCmd(_cmd);
  }
  else if (_cmd[0] == '>' || _cmd[0] == '@')
  {
    ControlCmd(_cmd);
  }
  /*---------------------------- ↑ Add Your CMDs Here ↑ -----------------------------*/
}

void OnUart2AsciiCmd(const char* _cmd, size_t _len, StreamSink& _responseChannel)
{
  /*---------------------------- ↓ Add Your CMDs Here ↓ -----------------------------*/
  if (_cmd[0] == '!')
  {
    CommonAsciiCmd(_cmd, _responseChannel);
  }
  else if (_cmd[0] == '#')
  {
    CommandModeCmd(_cmd);
  }
  else if (_cmd[0] == '>' || _cmd[0] == '@')
  {
    ControlCmd(_cmd);
  }
  /*---------------------------- ↑ Add Your CMDs Here ↑ -----------------------------*/
}
