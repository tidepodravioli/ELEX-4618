#include <../Common/CControl.h>
#include <CSerialCom.hpp>
#include <CSerialResponse.hpp>
#include <string>

class CControl2 : public CControl
{
    public:
    CControl2(int comport);
    bool send_com(CSerialCom &command);
    bool recv_com(CSerialResponse &response);
}