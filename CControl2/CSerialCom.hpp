#include <string>
#include <sstream>
#include <vector>

#define CSERIALCOM_GET_CHAR 'G' ///< Character representing a GET command on the serial bus
#define CSERIALCOM_SET_CHAR 'S'///< Character representing a SET command on the serial bus
#define CSERIALCOM_ACK_CHAR 'A'///< Character representing an ACK on the serial bus

#define CSERIALCOM_SPACE_CHAR ' '


using namespace std;

namespace CSerial
{
    /**
     * @brief Type of command being sent/received
     * 
     */
    enum COMMAND_TYPE
    {
        COMMAND_GET, ///< A get command
        COMMAND_SET, ///< A set command
        COMMAND_ACK, ///< An acknowledgement
        COMMAND_UNF ///< An unrecognized command
    };

    const int COMMAND_DATA_MAX = 2;
    /**
     * @brief Type of data being sent/received
     * 
     */
    enum COMMAND_DATA
    {
        DATA_UNF = -1,
        DATA_DIGITAL,
        DATA_ANALOG,
        DATA_SERVO
    };

    std::vector<string> delimitString(const string& input, char delimiter);
    COMMAND_TYPE parseType(char &letter);
    COMMAND_DATA parseDatatype(string &type);
}

using namespace CSerial;

class CSerialCom
{
    protected:
    COMMAND_TYPE m_commandType; ///< Type of command this object represents
    COMMAND_DATA m_commandData; ///< Type of data this object's command deals with
    int m_channel; ///< The channel the data/value is from/being sent to
    int m_value; ///< The data/value written/read

    public:

    /**
     * @brief Construct a new CSerialCom object, using a string command 
     * 
     * @param command The command as a string
     * @param valid Whether or not the resulting CSerialCom is valid as parsed
     */
    CSerialCom(string &command, bool &valid);
   
    /**
     * @brief Construct a new CSerialCom object, using given parameters to parse
     * 
     * @param type The type of command (GET, SET, ACK)
     * @param datatype The type of data being dealt with (DIG, ALG, SRV)
     * @param channel The channel the data is from/going to
     * @param value The value written/read
     */
    CSerialCom(COMMAND_TYPE type, COMMAND_DATA datatype, int channel, int value = -1);
    

    /**
     * @brief Gets the command as a string to be sent over the serial port
     * 
     * @param lf Whether or not to add a line feed character at the end of the command
     * @return string the command
     */
    string get_command(bool lf = false);
};