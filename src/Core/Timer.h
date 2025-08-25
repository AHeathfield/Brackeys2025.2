#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

class Timer
{
    public:
        //Initializes variables
        Timer()
            : mStartTicks(0),
              mPausedTicks(0),
              mPaused(false),
              mStarted(false)
        {

        }

        //The various clock actions
        void start()
        {
            mStarted = true;
            mPaused = false;

            // Get the current clock time
            mStartTicks = SDL_GetTicksNS();
            mPausedTicks = 0;
        }

        void stop()
        {
            mStarted = false;
            mPaused = false;

            mStartTicks = 0;
            mPausedTicks = 0;
        }

        void reset()
        {
            stop();
            start();
        }

        void pause()
        {
            //If the timer is running and isn't already paused
            if( mStarted && !mPaused )
            {
                //Pause the timer
                mPaused = true;

                //Calculate the paused ticks
                mPausedTicks = SDL_GetTicksNS() - mStartTicks;
                mStartTicks = 0;
            }
        }

        void unpause()
        {
            //If the timer is running and paused
            if( mStarted && mPaused )
            {
                //Unpause the timer
                mPaused = false;

                //Reset the starting ticks
                mStartTicks = SDL_GetTicksNS() - mPausedTicks;

                //Reset the paused ticks
                mPausedTicks = 0;
            }
        }

        //Gets the timer's time
        Uint64 getTicksNS()
        {
            //The actual timer time
            Uint64 time{ 0 };

            if( mStarted )
            {
                if( mPaused )
                {
                    //Return the number of ticks when the timer was paused
                    time = mPausedTicks;
                }
                else
                {
                    //Return the current time minus the start time
                    time = SDL_GetTicksNS() - mStartTicks;
                }
            }

            return time;
        }

        float getTimeS()
        {
            return (float) getTicksNS() / 1000000000;
        }

        //Checks the status of the timer
        bool isStarted()
        {
            return mStarted;
        }

        bool isPaused()
        {
            return mPaused;
        }

    private:
        //The clock time when the timer started
        Uint64 mStartTicks;

        //The ticks stored when the timer was paused
        Uint64 mPausedTicks;

        //The timer status
        bool mPaused;
        bool mStarted;
};

