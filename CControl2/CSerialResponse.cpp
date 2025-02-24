#include <CSerialResponse.hpp>

bool CSerialResponse::validate(CSerialCom &expected)
{
    const bool datatype = m_commandData == expected.m_commandData;
    const bool channel = m_channel == expected.m_channel;

    return datatype && channel;
}