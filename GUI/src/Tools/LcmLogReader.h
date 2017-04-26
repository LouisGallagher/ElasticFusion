#ifndef LCMLOGREADER_H_
#define LCMLOGREADER_H_

#include <lcm/lcm-cpp.hpp>

#include "LogReader.h"
#include "lcmtypes/eflcm/Frame.hpp"

class LcmLogReader : public LogReader
{
	public:
		LcmLogReader(std::string name)
		:LogReader(name, false),
		 lf(name, "r"),
		 done(false)
		{
			if(!lf.good())
			{
				std::exit(EXIT_FAILURE);
			}

			currentFrame = 0;

			depthReadBuffer = new unsigned char[Resolution::getInstance().numPixels() * 2];
			imageReadBuffer = new unsigned char[Resolution::getInstance().numPixels() * 3];
		}
		virtual ~LcmLogReader()
		{
			delete [] depthReadBuffer;
			delete [] imageReadBuffer;
		}

		void getNext()
		{
			const lcm::LogEvent * le = lf.readNextEvent();

			if(le == NULL)
			{
				done = true;
				return;
			}

			eflcm::Frame f;
			if(f.decode(le->data, 0, le->datalen) < 0)
			{
				return;
			}

			memcpy(&depthReadBuffer[0], f.depth.data(), f.depthSize);
			memcpy(&imageReadBuffer[0], f.image.data(), f.imageSize);

			depth = (unsigned short*)depthReadBuffer;
			rgb = (unsigned char*)imageReadBuffer;

			timestamp = f.timestamp;
			currentFrame++;
		}

        int getNumFrames()
        {
			return std::numeric_limits<int>::max();
        }

        bool hasMore()
        {
        	return !done;
        }

        bool rewound()
        {
        	return false;
        }

        void rewind()
        {
        	
        	lf.seekToTimestamp(0);
        	done = false;

        }

        void getBack(){}

        void fastForward(int frame){}

        const std::string getFile()
        {
        	return file;
        }

        void setAuto(bool value){}

		virtual bool getCameraIntrinsics(Eigen::Matrix3d & outIntrinsics)
		{
			return false;
		}

    private:
    	lcm::LogFile lf;

    	bool done;
};
#endif /*LCMLOGREADER_H_*/