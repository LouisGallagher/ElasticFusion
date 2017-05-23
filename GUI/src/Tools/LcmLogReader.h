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
			decompressionBufferDepth = new Bytef[Resolution::getInstance().numPixels() * 2];
			decompressionBufferImage =  new Bytef[Resolution::getInstance().numPixels() * 3];
		}
		virtual ~LcmLogReader()
		{
			delete [] depthReadBuffer;
			delete [] imageReadBuffer;
			delete [] decompressionBufferDepth;
			delete [] decompressionBufferImage;
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

			if(f.compressed)
			{
				unsigned long decompressedDepthSize = Resolution::getInstance().numPixels() * 2;
				uncompress(&decompressionBufferDepth[0], (unsigned long*)&decompressedDepthSize, (const Bytef*)(f.depth.data()), f.depthSize);		

				jpeg.readData((unsigned char *)f.image.data(), f.imageSize, (unsigned char *)&decompressionBufferImage[0]);

				memcpy(depthReadBuffer, (unsigned short *)decompressionBufferDepth, Resolution::getInstance().numPixels() * 2);
				memcpy(imageReadBuffer, (unsigned char *)decompressionBufferImage, Resolution::getInstance().numPixels() * 3);
			}
			else
			{
				memcpy(depthReadBuffer, f.depth.data(), f.depthSize);
				memcpy(imageReadBuffer, f.image.data(), f.imageSize);
			}

			depth = (unsigned short *) depthReadBuffer;
			rgb = (unsigned char *) imageReadBuffer;

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