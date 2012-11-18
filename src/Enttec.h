//
//  DmxGroup.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/13/12.
//
//

#ifndef motherfarmLED_DmxGroup_h
#define motherfarmLED_DmxGroup_h
class Enttec
{
private:
    unsigned char * data;
public:
    
    Enttec( int groupNo) : no( groupNo)
    {
        totalDataSize = 0;
    }
    
    int no;
    string ipAddress;
    vector< LocationLine > lines;
    int totalDataSize;
    void addLine( LocationLine& line)
    {
        lines.push_back( line );
    }
    
    string getIpAddress()
    {
        int ipPart = 120+no;
        return "192.168.11." + ofToString(ipPart);
    }
    
    void allocateData()
    {
        for( int i=0; i < lines.size(); i++ )
        {
            totalDataSize += lines[ i ].getDataSize();
        }
        data = new unsigned char[ totalDataSize ];
    }
    
    void copyData()
    {
        int size = 0;
        int offset = 0;
        for( int j=0; j < lines.size(); j++ )
        {
            size += lines[j].getDataSize();
            offset += j == 0 ? 0 : lines[j-1].getDataSize();
            memcpy(data + offset, lines[j].getData(), lines[j].getDataSize());
        }
    
    }
    
    void setData( int dataNo, int color)
    {
        data[ dataNo * 3 ] = (color >> 16) & 0xff;
        data[ dataNo * 3 + 1 ] = (color >> 8) & 0xff;
        data[ dataNo * 3 + 2 ] = color & 0xff;
    }
    
    void setAllData(int color)
    {
        for ( int i = 0; i < lines.size(); i++)
        {
            unsigned char* dt = lines[i].getData();
            int size = lines[i].getDataSize();
            for( int j = 0; j < size; j++)
            {
                dt[j] = 255;
            }
        }
    }
    
    unsigned char* getData()
    {
        if( false )
        {
            for( int i=1; i < lines.size(); i++ )
            {
                if ( i == 2)
                {
                cout << (int)data[lines[i-1].getDataSize()] << " " << (int)data[lines[i-1].getDataSize() + 1] << endl;
//
//                cout << "_"<<(int)data[lines[i-1].getDataSize() + 2] << " " << (int)data[lines[i-1].getDataSize() + 3] << endl;
                }
            }
        }
        return data;
    }
    
};
#endif
