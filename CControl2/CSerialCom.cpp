#include <CSerialCom.hpp>

CSerialCom::CSerialCom(string &command, bool &valid)
{
    vector<string> parts = CSerial::delimitString(command, CSERIALCOM_SPACE_CHAR);
        
    if(parts.size() >= 3)
    {
        try
        {
            m_commandType = CSerial::parseType(parts[0].at(0)); // get command type
            m_commandData = CSerial::parseDatatype(parts[1]); //get datatype
            m_channel = stoi(parts[2]); //get channel
            if(parts.size() == 4)
                m_value = stoi(parts[3]);
        }
        catch(const std::exception& e)
        {
            valid = false;
        }
        
        
    }
}

CSerialCom:: CSerialCom(COMMAND_TYPE type, COMMAND_DATA datatype, int channel, int value)
{
    m_commandType = type;
    m_commandData = datatype;
    m_channel = channel;
    m_value = value;
}

string CSerialCom::get_command(bool lf)
{
    stringstream command;
    command << returnType(m_commandType) << CSERIAL_SPACE_CHAR;
    command << (int)m_commandData << CSERIAL_SPACE_CHAR;
    command << m_channel;

    if(m_value != -1)
        command << m_value;
    
    if(lf) command << '\n';
    
    return command.str();
}



namespace CSerial
{
    vector<string> delimitString(const string& input, char delimiter) {
        vector<string> parts;
        stringstream ss(input);
        string part;
    
        while (getline(ss, part, delimiter)) {
            parts.push_back(part);
        }
    
        // Handle the case where the last character is the delimiter
        if (!input.empty() && input.back() == delimiter) {
            parts.push_back("");
        }
    
        return parts;
    }

    COMMAND_TYPE parseType(char &letter)
    {
        switch(letter)
        {
            case CSERIALCOM_GET_CHAR:
                return COMMAND_GET;
            case CSERIALCOM_SET_CHAR:
                return COMMAND_SET;
            case CSERIALCOM_ACK_CHAR:
                return COMMAND_ACK;
            default:
                return COMMAND_UNF;
        }
    }

    char returnType(COMMAND_TYPE type)
    {
        switch(type)
        {
            case COMMAND_GET:
                return CSERIALCOM_GET_CHAR;
            case COMMAND_SET:
                return CSERIALCOM_SET_CHAR;
            case COMMAND_ACK:
                return CSERIALCOM_ACK_CHAR;
            default:
                return CSERIALCOM_SPACE_CHAR;
        }
    }

    COMMAND_DATA parseDatatype(string &type)
    {
        int dataTypeNum = -1;
        try
        {
            dataTypeNum = stoi(type);
        }
        catch(const std::exception& e)
        {
            return DATA_UNF;
        }

        if(dataTypeNum >= 0 && dataTypeNum <= COMMAND_DATA_MAX)
            return (COMMAND_DATA)dataTypeNum;
        
        
        return DATA_UNF;
    }
}