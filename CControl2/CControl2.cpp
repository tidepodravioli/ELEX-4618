#include <CControl2.hpp>

CControl2::CControl2(int comport)
{

}

bool CControl2::send_com(CSerialCom &command)
{
    // init 
    const std::string commandText = command.get_command();
    const int bufferLength = commandText.size();

    // writes if the port is open
    if(m_com.is_open())
    {
        m_com.write(commandText.c_str(), bufferLength);
        return true;
    }
    else return false;
}

bool CControl2::recv_com(CSerialResponse &response)
{
    if(m_com.is_open())
    {
        // Creates a buffer for the acknowledgement message
        char * acknowledgement = new char[CHAR_ARRAY_MAX_SIZE];
        int sizeAck = -1;

        // Reads the message from the serial monitor when it's available
        auto startTime = std::chrono::steady_clock::now();
        while(true)
        {
            sizeAck = m_com.read(acknowledgement, CHAR_ARRAY_MAX_SIZE);

            if(sizeAck > 0) break;

            auto elapsed_time = std::chrono::steady_clock::now() - startTime;
            if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() > SERIAL_PORT_TIMEOUT) {
                return false;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(SERIAL_PORT_DELAY));
        }
        m_com.flush();

        string message = acknowledgement;
        message = message.substr(0, sizeAck - 1);

        bool valid = false;
        response = CSerialResponse(message, valid);

        return valid;
    }
    else return false;
}
