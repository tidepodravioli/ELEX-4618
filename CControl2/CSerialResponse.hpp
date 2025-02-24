#include <CSerialCom.hpp>

class CSerialResponse : public CSerialCom
{
    public:
    CSerialResponse(string &response, bool &valid);
    
    /**
     * @brief Validates the response by making sure that it originated from
     * the intended channel and bus (datatype)
     * 
     * @param expected A dummy packet with the intended channel and datatype (ignores other data)
     * @return true 
     * @return false 
     */
    bool validate(CSerialCom &expected);
    int get_channel() { return m_channel; }
    int get_value() { return m_value; }
};