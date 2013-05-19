/*
 *   Laser.cpp
 *
 *   Author: Wu Chih-En
 *
 */
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include "Laser.h"

using namespace Robot;

//#define INST_PING			(1)
//#define INST_READ			(2)
//#define INST_WRITE			(3)
//#define INST_REG_WRITE		(4)
//#define INST_ACTION			(5)
//#define INST_RESET			(6)
//#define INST_SYNC_WRITE		(131)   // 0x83
//#define INST_BULK_READ      (146)   // 0x92


Laser::Laser(SensorLaser *sensor)
{
	mpSensor = sensor;
	
	mMaxScanRange  = ( STOP_STEP - START_STEP ) * MIN_DEGREE;
    mMaxStartAngle = ( START_STEP - FRONT_STEP ) * MIN_DEGREE;
    mMaxStopAngle  = ( STOP_STEP  - FRONT_STEP ) * MIN_DEGREE;
	
    Initialize(START_STEP,STOP_STEP,1,0,0);
	
    mPastTimeStamp = 1;
    mFreq    = 0;
    mPkgSize = 0;
	
	this->mRecevingFlag = false;
}

Laser::~Laser()
{
	Disconnect();
}

int Laser::RxPacket(unsigned char *rxpacket) {
		
	int res = RX_FAIL;
	
	int length;
	
	mpSensor->SetPacketTimeout(mCmdLen);
	
	int get_length = 0;
	if(DEBUG_PRINT == true)
		fprintf(stderr, "RX: ");
	
	while(1)
	{
		length = mpSensor->ReadPort(&rxpacket[get_length], sizeof(unsigned char));
		if(DEBUG_PRINT == true)
		{
			for(int n=0; n<length; n++)
				fprintf(stderr, "%.2X ", rxpacket[get_length + n]);
		}
		get_length += length;
	
		if(rxpacket[get_length-1] == ASCII_LF && rxpacket[get_length-2] == ASCII_LF)
		{
			res = SUCCESS;
			break;
		}
		else
		{
			if(mpSensor->IsPacketTimeout() == true)
			{
				if(get_length == 0)
					res = RX_TIMEOUT;
				else
					res = RX_CORRUPT;
				
				break;
			}
		}
	}
	
	if(DEBUG_PRINT == true)
	{
		fprintf(stderr, "Time:%.2fms  ", mpSensor->GetPacketTime());
		fprintf(stderr, "RETURN: ");
		switch(res)
		{
			case SUCCESS:
				fprintf(stderr, "RX_SUCCESS\n");
				break;
	
			case RX_FAIL:
				fprintf(stderr, "RX_FAIL\n");
				break;
	
			case RX_TIMEOUT:
				fprintf(stderr, "RX_TIMEOUT\n");
				break;
	
			case RX_CORRUPT:
				fprintf(stderr, "RX_CORRUPT\n");
				break;
	
			default:
				fprintf(stderr, "UNKNOWN\n");
				break;
		}
	}
	
	return res;
}

int Laser::TxPacket(unsigned char *txpacket, int priority)
{
	if(priority > 1)
		mpSensor->LowPriorityWait();
	if(priority > 0)
		mpSensor->MidPriorityWait();
	mpSensor->HighPriorityWait();

	int res = TX_FAIL;

	if(DEBUG_PRINT == true)
	{
		fprintf(stderr, "\nTX: ");
		for(int n=0; n<mCmdLen; n++)
			fprintf(stderr, "%.2X ", txpacket[n]);

		//fprintf(stderr, "INST: ");
		//switch(txpacket[INSTRUCTION])
		//{
		//case INST_PING:
		//	fprintf(stderr, "PING\n");
		//	break;

		//case INST_READ:
		//	fprintf(stderr, "READ\n");
		//	break;

		//case INST_WRITE:
		//	fprintf(stderr, "WRITE\n");
		//	break;

		//case INST_REG_WRITE:
		//	fprintf(stderr, "REG_WRITE\n");
		//	break;

		//case INST_ACTION:
		//	fprintf(stderr, "ACTION\n");
		//	break;

		//case INST_RESET:
		//	fprintf(stderr, "RESET\n");
		//	break;

		//case INST_SYNC_WRITE:
		//	fprintf(stderr, "SYNC_WRITE\n");
		//	break;

        //case INST_BULK_READ:
        //    fprintf(stderr, "BULK_READ\n");
        //    break;

		//default:
		//	fprintf(stderr, "UNKNOWN\n");
		//	break;
		//}
	}

	mpSensor->ClearPort();
	if(mpSensor->WritePort(txpacket, this->mCmdLen) == this->mCmdLen)
	{
		res = SUCCESS;
	}
	else
		res = TX_CORRUPT;

	if(DEBUG_PRINT == true)
	{
		fprintf(stderr, "RETURN: ");
		switch(res)
		{
		case SUCCESS:
			fprintf(stderr, "TX_SUCCESS\n");
			break;

		case TX_CORRUPT:
			fprintf(stderr, "TX_CORRUPT\n");
			break;

		case TX_FAIL:
			fprintf(stderr, "TX_FAIL\n");
			break;

		default:
			fprintf(stderr, "UNKNOWN\n");
			break;
		}
	}

	mpSensor->HighPriorityRelease();
    if(priority > 0)
        mpSensor->MidPriorityRelease();
    if(priority > 1)
        mpSensor->LowPriorityRelease();

	return res;
}
//---------------------------------------------------------------------------
void Laser::Initialize( int start, int end, int cluster, int interval, int times)
{
    cluster = (cluster > 1 ) ? cluster : 1;
    this->mScanNum = ( ( end - start ) / cluster ) + 1;      // scan lines

    this->mStartAngle = ( start - FRONT_STEP ) * MIN_DEGREE * M_PI / 180.0;
    this->mStopAngle  = ( end - FRONT_STEP ) * MIN_DEGREE * M_PI / 180.0;
    this->mScanScale  = cluster * MIN_DEGREE *M_PI / 180.0;
    this->mScanRange  = mStopAngle - mStartAngle;
	
	this->mScanTimes = times;
}
//---------------------------------------------------------------------------
void Laser::MakeBaseCommand(char C1 , char C2 , int length)
{
    this->mCmdLen = length;
	delete [] this->mpCmd;
    this->mpCmd = new unsigned char[ this->mCmdLen ];         //宣告新的命令矩陣

    this->mpCmd[0] = C1;  this->mpCmd[1] = C2;     //填入命令編號
    this->mpCmd[ this->mCmdLen-1 ] = ASCII_LF; //最後一筆為 LF
}
//---------------------------------------------------------------------------
void Laser::SendMdmsCommand( int start, int end, int cluster, int interval, int times)
{
	//===== 設定基本命令格式 ================================================
    this->MakeBaseCommand( 'M' ,'D' ,16 );

    Initialize( start, end, cluster, interval, times );

    mpScanAngle[0] = mStartAngle;
    for( int i = 1 ; i < MAX_SCAN_SIZE ; i++ )
    {
        if( i < this->mScanNum )
        {
            mpScanAngle[i] = mpScanAngle[i-1] + this->mScanScale;
        }
        else{ mpScanAngle[i] = -999; }
    }

    int data_len = this->mScanNum * 3;
    mPkgSize = data_len + 29 + 2*( (int)( data_len )/64 );

    //===== 命令參數轉成封包格式 =============================================
    int start_tmp = start;
    int end_tmp   = end;

    for( int r = 5 ; r >= 2 ; r-- )
    {
        this->mpCmd[ r ] = hex2ascii((char)( start_tmp % 10) );
        start_tmp = start_tmp / 10;
    }
    for( int r = 9 ; r >= 6 ; r-- )
    {
        this->mpCmd[ r ] = hex2ascii((char)( end_tmp % 10) );
        end_tmp   = end_tmp / 10;
    }
    this->mpCmd[10] = hex2ascii((char)( cluster / 10) );
    this->mpCmd[11] = hex2ascii((char)( cluster % 10) );

    this->mpCmd[12] = hex2ascii((char) interval );

    this->mpCmd[13] = hex2ascii((char)( times / 10) );
    this->mpCmd[14] = hex2ascii((char)( times % 10) );

	TxPacket(this->mpCmd, 0);
	
}
//---------------------------------------------------------------------------
void Laser::SendQtCommand( void )
{
    mFreq = 0;
    this->MakeBaseCommand( 'Q', 'T', 3 );
	mpSensor->ClearPort();
	TxPacket(this->mpCmd, 0);
	
}
//---------------------------------------------------------------------------
void Laser::SendRsCommand( void )
{
    mFreq = 0;
    this->MakeBaseCommand( 'R', 'S', 3 );
	TxPacket(this->mpCmd, 0);
	
}
//---------------------------------------------------------------------------
void Laser::SendBmCommand( void )
{
    this->MakeBaseCommand( 'B', 'M', 3 );
	TxPacket(this->mpCmd, 0);
	
}
//---------------------------------------------------------------------------
void Laser::SendVvCommand( void )
{
    this->MakeBaseCommand( 'V', 'V', 3 );
	TxPacket(this->mpCmd, 0);
	
}
//---------------------------------------------------------------------------
void Laser::SendPpCommand( void )
{
    this->MakeBaseCommand( 'P', 'P', 3 );
	TxPacket(this->mpCmd, 0);
	
}
//---------------------------------------------------------------------------
void Laser::SendIiCommand( void )
{
    this->MakeBaseCommand( 'I', 'I', 3 );
	TxPacket(this->mpCmd, 0);
	
}
//---------------------------------------------------------------------------
const char *Laser::AnalyzePacket( const unsigned char *p_data_stream , int length )
{
    const char *Error = NULL;
    mFreq = 0;
    switch( p_data_stream[0] )
    {
    case 'M':
		if( p_data_stream[1]=='D' || p_data_stream[1]=='S' ) {
			Error = "MDMS-Command";
			AnalyzeMDMS( p_data_stream, length );
			break;
		}
    case 'G': if( p_data_stream[1]=='D' || p_data_stream[1]=='S' ){ Error = "GDGS-Command"; break; }
    case 'B': if( p_data_stream[1]=='M' ) { Error = "BM-Command"; this->mStatus = -1; break; }
    case 'Q': if( p_data_stream[1]=='T' ) { Error = "QT-Command"; this->mStatus = -1; break; }
    case 'R': if( p_data_stream[1]=='S' ) { Error = "RS-Command"; this->mStatus = -1; break; }
    case 'T': if( p_data_stream[1]=='M' ) { Error = "TM-Command"; this->mStatus = -1; break; }
    case 'S': if( p_data_stream[1]=='S' ) { Error = "SS-Command"; this->mStatus = -1; break; }
    case 'D': if( p_data_stream[1]=='B' ) { Error = "DB-Command"; this->mStatus = -1; break; }
    case 'V': if( p_data_stream[1]=='V' ) { Error = "VV-Command"; this->mStatus = -1; break; }
    case 'P': if( p_data_stream[1]=='P' ) { Error = "PP-Command"; this->mStatus = -1; break; }
    case 'I': if( p_data_stream[1]=='I' ) { Error = "II-Command"; this->mStatus = -1; break; }
    default:
        Error = "Unknown Data";
    }
    return Error;
}
//---------------------------------------------------------------------------
void Laser::AnalyzeMDMS( const unsigned char *p_data_stream , int length )
{
	this->mStatus = CheckStatus( p_data_stream, length);
	if(this->mStatus == 99) {
		// buffer for decoding data
		char buffer[3];                // 資料解碼暫存器
		short buffer_size = p_data_stream[1]=='D' ? 3 : 2; // C3 decoding for "MS", C2 decoding for "MD"
		short buffer_cnt = 0;              // 解碼計數器
		short byte_cnt = 0;                // 位元計數器 ,每64位元則重新計數
		short data_cnt = 0;                // 資料計數器 ,每解碼出一條掃描線則增加 1
		
		buffer[0] = 0;  buffer[1] = 0; buffer[2] = 0;
		
		//==== 第20~23筆為 時間標記
		mTimeStamp = four_char_decoding( p_data_stream[20],p_data_stream[21],p_data_stream[22],p_data_stream[23] );
		mFreq = ((mTimeStamp - mPastTimeStamp) == 0 )? 0 : 1000 / (mTimeStamp - mPastTimeStamp);
		mPastTimeStamp = mTimeStamp;
		//==== 第24筆為 CheckSum
		//==== 第25筆為 LF
		//==== 第26筆之後 到 倒數第3筆之前 為掃描線資料，倒數第三筆為 Checksum 倒數兩筆為 LF LF
		for( int i = 26 ; i < mPkgSize - 3 ; i++ )
		{
			// if scan data bigger than 64 bytes, there will be a checksum and a LF for every 64-byte
			if( byte_cnt < 64 )
			{
				buffer[buffer_cnt] = p_data_stream[i];
				//=== 解碼長度足夠，將資料存入資料矩陣
				if( ++buffer_cnt >= buffer_size  )
				{
					mpScanArray[ (this->mScanNum-1) - data_cnt++ ] = three_char_decoding( buffer[0] ,buffer[1] ,buffer[2] )/ SCALE;
					buffer_cnt = 0;
				}
				byte_cnt++;
			}
			else
			{
				byte_cnt = 0;
				i++;
			}
		}
	}
}

short Laser::CheckStatus( const unsigned char *p_data_stream, int length) {
	return ascii2hex( p_data_stream[16] )*10 + ascii2hex( p_data_stream[17] );
}

const char *Laser::GetStatusString( void ) {
	switch( this->mStatus ){
		case  0: return "Command received without any Error";
		case  1: return "Starting Step has non-numeric value";
		case  2: return "End Step has non-numeric value";
		case  3: return "Cluster Count has non-numeric value";
		case  4: return "End Step is out of range";
		case  5: return "End Step is smaller than Starting Step";
		case  6: return "Scan Interval has non-numeric value";
		case  7: return "Number of Scan has non-numeric value";
		case 98: return "Resumption of process after confirming normal laser operation";
		case 99: return "MDMS-Command";
		default:
			if( mStatus >= 21 && mStatus <= 49 )
				return "Processing stopped to verify the error";
			else if( mStatus >= 50 && mStatus <= 97 )
				return "Hardware trouble (such as laser, motor malfunctions etc)";
			else
				return "Unknown mStatus";
    }
}

bool Laser::Connect()
{
	if(mpSensor->OpenPort() == false)
	{
        fprintf(stderr, "\n Fail to open port\n");
        fprintf(stderr, " Laser is used by another program or do not have root privileges.\n\n");
		return false;
	}
	
	StartReceive();
	return true;
}

bool Laser::ChangeBaud(int baud)
{
    if(mpSensor->SetBaud(baud) == false)
    {
        fprintf(stderr, "\n Fail to change baudrate\n");
        return false;
    }
    return true;
}

void Laser::Disconnect()
{
	pthread_cancel(mRecevieThread);
	mpSensor->ClosePort();
}

int Laser::StartReceive()
{
    int result;

	this->mRecevieThread = -1;
	
    result = pthread_create(&this->mRecevieThread, NULL, this->ReceiveThreadProc, this);
    if(result < 0)
        fprintf(stderr, "Main Routine thread start fail!!\n");
    pthread_detach(mRecevieThread);

    return 0;
}

void* Laser::ReceiveThreadProc(void* param)
{
	Laser *self = (Laser*)param;

	while(1) {
		self->mRecevingFlag = true;
		self->RxPacket(self->PkgData);
		self->mRecevingFlag = false;
		self->AnalyzePacket(self->PkgData, self->mPkgSize);
	}
	
    return 0;
}
