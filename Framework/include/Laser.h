/*
 *   Laser.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _LASER_H_
#define _LASER_H_

#define ASCII_LF 0x0A

#define START_STEP 0
#define STOP_STEP 1080
#define FRONT_STEP 540

#define MIN_DEGREE 0.25
#define MAX_SCAN_SIZE 1081
#define SCALE 1.0

namespace Robot
{
	inline unsigned char hex2ascii( const char &hex ){ return (( hex & 0x3f ) + 0x30); }
	inline unsigned char ascii2hex( const char &ascii  ){ return (( ascii - 0x30 ) & 0x3f); }
	inline int four_char_decoding( const char &c1,const char &c2,const char &c3,const char &c4)
	{
		return  ( (((c1-0x30) & 0x3f) << 18) +
				  (((c2-0x30) & 0x3f) << 12) +
				  (((c3-0x30) & 0x3f) << 6) +
				  ((c4-0x30) & 0x3f)  )&0x00ffffff;
	}
	//---------------------------------------------------------------------------
	inline int three_char_decoding( const char& c1,const char& c2,const char& c3 )
	{
		return  ( (((c1-0x30) & 0x3f) << 12) +
				  (((c2-0x30) & 0x3f) << 6) +
				  ((c3-0x30) & 0x3f)  )&0x0003ffff;
	}
	//---------------------------------------------------------------------------
	inline int two_char_decoding( const char& c1,const char& c2 )
	{
		return  ( (((c1-0x30) & 0x3f) << 6) +
				  ((c2-0x30) & 0x3f)  )&0x00000fff;
	}

	class SensorLaser
	{
	public:
		/////////// Need to implement below methods (Platform porting) //////////////
		// Port control
		virtual bool OpenPort() = 0;
        virtual bool SetBaud(int baud) = 0;
		virtual void ClosePort() = 0;
		virtual void ClearPort() = 0;
		virtual int WritePort(unsigned char* packet, int numPacket) = 0;
		virtual int ReadPort(unsigned char* packet, int numPacket) = 0;

		// Using semaphore
		virtual void LowPriorityWait() = 0;
		virtual void MidPriorityWait() = 0;
		virtual void HighPriorityWait() = 0;
		virtual void LowPriorityRelease() = 0;
		virtual void MidPriorityRelease() = 0;
		virtual void HighPriorityRelease() = 0;

		// Using timeout
		virtual void SetPacketTimeout(int lenPacket) = 0;
		virtual bool IsPacketTimeout() = 0;
		virtual double GetPacketTime() = 0;
		virtual void SetUpdateTimeout(int msec) = 0;
		virtual bool IsUpdateTimeout() = 0;
		virtual double GetUpdateTime() = 0;

		virtual void Sleep(double msec) = 0;
		//////////////////////////////////////////////////////////////////////////////
	};

	class Laser
	{
	public:
		enum
		{
			SUCCESS,
			TX_CORRUPT,
			TX_FAIL,
			RX_FAIL,
			RX_TIMEOUT,
			RX_CORRUPT
		};
		
	private:
		SensorLaser *mpSensor;
		
		void MakeBaseCommand(char, char, int );
		
		float mMaxScanRange;
		float mMaxStartAngle;
		float mMaxStopAngle;
		
		int mTimeStamp;
		int mFreq;
		short mStatus;
		
		// MDMS Command Parameter
		unsigned char *mpCmd;
		int mCmdLen;
		int mScanTimes;
		
		int mPastTimeStamp;
		
		// Recevie Thread
		pthread_t mRecevieThread;
		bool mRecevingFlag;
		int StartReceive();
		static void* ReceiveThreadProc(void*);

		int TxPacket(unsigned char *, int); 
		int RxPacket(unsigned char *);
		
		const char* AnalyzePacket( const unsigned char *, int );
		short CheckStatus( const unsigned char *, int );
		void AnalyzeMDMS( const unsigned char *, int);
		
		int mScanNum;
		float mScanScale;
		
		float mStartAngle;
		float mStopAngle;
		
		float mScanRange;
		int mpScanArray[ MAX_SCAN_SIZE ];
		float mpScanAngle[ MAX_SCAN_SIZE ];

		int  mPkgSize;
		unsigned char PkgData[16860];
		
	public:
		static const bool DEBUG_PRINT = true;

		Laser(SensorLaser *sensor);
		~Laser();

		bool Connect();
        bool ChangeBaud(int baud);
		void Disconnect();

		void Initialize(int, int, int, int, int);
		
		void SendMdmsCommand( int, int, int, int, int );
		void SendQtCommand();
		void SendRsCommand();
		void SendBmCommand();
		void SendVvCommand();
		void SendPpCommand();
		void SendIiCommand();
		
		//===== Output Function ==============
		inline int GetCommandLength( void ){ return this->mCmdLen; }
		inline float GetTimeStamp( void ){ return this->mTimeStamp; }
		inline int GetFreq( void ){return this->mFreq; }
		inline float GetStatus( void ){ return this->mStatus; }
        inline int GetScanNum() { return this->mScanNum; }
        inline float GetScanScale() { return this->mScanScale; }
		inline bool IsReceving( void ) { return this->mRecevingFlag; }
		
		const char *GetStatusString( void );
		
	};
}

#endif
