/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SYSTEM_H
#define SYSTEM_H

#include "Map.h"
#include "ORBVocabulary.h"
#include "IPublisherThread.h"

#include <string>
#include <thread>
#include <mutex>
#include <opencv2/core/core.hpp>


#include "BoostArchiver.h"


enum __rlimit_resource
{
  /* Per-process CPU limit, in seconds.  */
  RLIMIT_CPU = 0,
#define RLIMIT_CPU RLIMIT_CPU

  /* Largest file that can be created, in bytes.  */
  RLIMIT_FSIZE = 1,
#define	RLIMIT_FSIZE RLIMIT_FSIZE

  /* Maximum size of data segment, in bytes.  */
  RLIMIT_DATA = 2,
#define	RLIMIT_DATA RLIMIT_DATA

  /* Maximum size of stack segment, in bytes.  */
  RLIMIT_STACK = 3,
#define	RLIMIT_STACK RLIMIT_STACK

  /* Largest core file that can be created, in bytes.  */
  RLIMIT_CORE = 4,
#define	RLIMIT_CORE RLIMIT_CORE

  /* Largest resident set size, in bytes.
     This affects swapping; processes that are exceeding their
     resident set size will be more likely to have physical memory
     taken from them.  */
  __RLIMIT_RSS = 5,
#define	RLIMIT_RSS __RLIMIT_RSS

  /* Number of open files.  */
  RLIMIT_NOFILE = 7,
  __RLIMIT_OFILE = RLIMIT_NOFILE, /* BSD name for same.  */
#define RLIMIT_NOFILE RLIMIT_NOFILE
#define RLIMIT_OFILE __RLIMIT_OFILE

  /* Address space limit.  */
  RLIMIT_AS = 9,
#define RLIMIT_AS RLIMIT_AS

  /* Number of processes.  */
  __RLIMIT_NPROC = 6,
#define RLIMIT_NPROC __RLIMIT_NPROC

  /* Locked-in-memory address space.  */
  __RLIMIT_MEMLOCK = 8,
#define RLIMIT_MEMLOCK __RLIMIT_MEMLOCK

  /* Maximum number of file locks.  */
  __RLIMIT_LOCKS = 10,
#define RLIMIT_LOCKS __RLIMIT_LOCKS

  /* Maximum number of pending signals.  */
  __RLIMIT_SIGPENDING = 11,
#define RLIMIT_SIGPENDING __RLIMIT_SIGPENDING

  /* Maximum bytes in POSIX message queues.  */
  __RLIMIT_MSGQUEUE = 12,
#define RLIMIT_MSGQUEUE __RLIMIT_MSGQUEUE

  /* Maximum nice priority allowed to raise to.
     Nice levels 19 .. -20 correspond to 0 .. 39
     values of this resource limit.  */
  __RLIMIT_NICE = 13,
#define RLIMIT_NICE __RLIMIT_NICE

  /* Maximum realtime priority allowed for non-priviledged
     processes.  */
  __RLIMIT_RTPRIO = 14,
#define RLIMIT_RTPRIO __RLIMIT_RTPRIO

  /* Maximum CPU time in µs that a process scheduled under a real-time
     scheduling policy may consume without making a blocking system
     call before being forcibly descheduled.  */
  __RLIMIT_RTTIME = 15,
#define RLIMIT_RTTIME __RLIMIT_RTTIME

  __RLIMIT_NLIMITS = 16,
  __RLIM_NLIMITS = __RLIMIT_NLIMITS
#define RLIMIT_NLIMITS __RLIMIT_NLIMITS
#define RLIM_NLIMITS __RLIM_NLIMITS
};

#ifndef __USE_FILE_OFFSET64
typedef __rlim_t rlim_t;
#else
typedef __rlim64_t rlim_t;
#endif
#ifdef __USE_LARGEFILE64
typedef __rlim64_t rlim64_t;
#endif

struct rlimit
  {
    /* The current (soft) limit.  */
    rlim_t rlim_cur;
    /* The hard limit.  */
    rlim_t rlim_max;
  };


#if defined __USE_GNU && !defined __cplusplus
typedef enum __rlimit_resource __rlimit_resource_t;
typedef enum __rusage_who __rusage_who_t;
typedef enum __priority_which __priority_which_t;
#else
typedef int __rlimit_resource_t;
typedef int __rusage_who_t;
typedef int __priority_which_t;
#endif
/* Put the soft and hard limits for RESOURCE in *RLIMITS.
   Returns 0 if successful, -1 if not (and sets errno).  */
#ifndef __USE_FILE_OFFSET64
extern int getrlimit (__rlimit_resource_t __resource,
		      struct rlimit *__rlimits) __THROW;
#else
# ifdef __REDIRECT_NTH
extern int __REDIRECT_NTH (getrlimit, (__rlimit_resource_t __resource,
				       struct rlimit *__rlimits), getrlimit64);
# else
#  define getrlimit getrlimit64
# endif
#endif
#ifdef __USE_LARGEFILE64
extern int getrlimit64 (__rlimit_resource_t __resource,
			struct rlimit64 *__rlimits) __THROW;
#endif

/* Set the soft and hard limits for RESOURCE to *RLIMITS.
   Only the super-user can increase hard limits.
   Return 0 if successful, -1 if not (and sets errno).  */
#ifndef __USE_FILE_OFFSET64
extern int setrlimit (__rlimit_resource_t __resource,
		      const struct rlimit *__rlimits) __THROW;
#else
# ifdef __REDIRECT_NTH
extern int __REDIRECT_NTH (setrlimit, (__rlimit_resource_t __resource,
				       const struct rlimit *__rlimits),
			   setrlimit64);
# else
#  define setrlimit setrlimit64
# endif
#endif
#ifdef __USE_LARGEFILE64
extern int setrlimit64 (__rlimit_resource_t __resource,
			const struct rlimit64 *__rlimits) __THROW;
#endif

namespace ORB_SLAM2
{

class IPublisherThread;
class IFrameSubscriber;
class IMapPublisher;
class Map;
class Tracking;
class LocalMapping;
class LoopClosing;
class KeyFrameDatabase;

class System
{
public:
    // Input sensor
    enum eSensor{
        MONOCULAR=0,
        STEREO=1,
        RGBD=2
    };

    // A `Builder`'s responsibility is to create, link together and destroy each individual
    // part of a `System`.  The pure virtual methods are all supposed to be simple getters.
    class IBuilder {
    public:
        virtual ~IBuilder();
        virtual eSensor GetSensorType() = 0;
        virtual ORBVocabulary* GetVocabulary() = 0;
        virtual KeyFrameDatabase* GetKeyFrameDatabase() = 0;
        virtual Map* GetMap() = 0;
        virtual Tracking* GetTracker() = 0;
        virtual LocalMapping* GetLocalMapper() = 0;
        virtual LoopClosing* GetLoopCloser() = 0;
        virtual IPublisherThread *GetPublisher() = 0;
    };

    // This `IBuilder` implementation  constructs a system completely with the exception of
    // the IPublisherThread and directly related objects (IFrameSubscriber, IMapPublisher).
    // Since different Builders usually differ only by the class of those publishing objects,
    // it's usually more practical to subclass this class instead of the IBuilder.
    class GenericBuilder : public IBuilder {
    public:
        GenericBuilder(const std::string &strVocFile, const std::string &strSettingsFile, eSensor sensor);
        virtual ~GenericBuilder();

        virtual eSensor GetSensorType() override;
        virtual ORBVocabulary *GetVocabulary() override;
        virtual KeyFrameDatabase *GetKeyFrameDatabase() override;
        virtual Map *GetMap() override;
        virtual Tracking *GetTracker() override;
        virtual LocalMapping *GetLocalMapper() override;
        virtual LoopClosing *GetLoopCloser() override;

    protected:
        eSensor mSensor;
        cv::FileStorage mSettings;
        ORBVocabulary mVocabulary;
        std::unique_ptr<KeyFrameDatabase> mpKeyFrameDatabase;
        std::unique_ptr<Map> mpMap;
        std::unique_ptr<Tracking> mpTracker;
        std::unique_ptr<LocalMapping> mpLocalMapper;
        std::unique_ptr<LoopClosing> mpLoopCloser;
    };

    // Creates the SLAM system.
    // The created `System` object takes ownership of the passed `Builder`.
    // The system will still need to be started by calling `System::Start()`.
    System(std::unique_ptr<IBuilder> builder);

    // Proccess the given stereo frame. Images must be synchronized and rectified.
    // Input images: RGB (CV_8UC3) or grayscale (CV_8U). RGB is converted to grayscale.
    // Returns the camera pose (empty if tracking fails).
    cv::Mat TrackStereo(const cv::Mat &imLeft, const cv::Mat &imRight, const double &timestamp);

    // Process the given rgbd frame. Depthmap must be registered to the RGB frame.
    // Input image: RGB (CV_8UC3) or grayscale (CV_8U). RGB is converted to grayscale.
    // Input depthmap: Float (CV_32F).
    // Returns the camera pose (empty if tracking fails).
    cv::Mat TrackRGBD(const cv::Mat &im, const cv::Mat &depthmap, const double &timestamp);

    // Proccess the given monocular frame
    // Input images: RGB (CV_8UC3) or grayscale (CV_8U). RGB is converted to grayscale.
    // Returns the camera pose (empty if tracking fails).
    cv::Mat TrackMonocular(const cv::Mat &im, const double &timestamp);

    void Start();

    // This stops local mapping thread (map building) and performs only camera tracking.
    void ActivateLocalizationMode();
    // This resumes local mapping thread and performs SLAM again.
    void DeactivateLocalizationMode();

    // To check if there is a need to perform scale correction (only when a monocular sensor is used)
    eSensor GetSensorType() { return mSensor; };

    // Returns true if there have been a big map change (loop closure, global BA)
    // since last call to this function
    bool MapChanged();

    // Reset the system (clear map)
    void Reset();

    // All threads will be requested to finish.
    // It waits until all threads have finished.
    // This function must be called before saving the trajectory.
    void Shutdown();

    // Save camera trajectory in the TUM RGB-D dataset format.
    // Only for stereo and RGB-D. This method does not work for monocular.
    // Call first Shutdown()
    // See format details at: http://vision.in.tum.de/data/datasets/rgbd-dataset
    void SaveTrajectoryTUM(const std::string &filename);

    // Save keyframe poses in the TUM RGB-D dataset format.
    // This method works for all sensor input.
    // Call first Shutdown()
    // See format details at: http://vision.in.tum.de/data/datasets/rgbd-dataset
    void SaveKeyFrameTrajectoryTUM(const std::string &filename);

    // Save camera trajectory in the KITTI dataset format.
    // Only for stereo and RGB-D. This method does not work for monocular.
    // Call first Shutdown()
    // See format details at: http://www.cvlibs.net/datasets/kitti/eval_odometry.php
    void SaveTrajectoryKITTI(const std::string &filename);

    // Information from most recent processed frame
    // You can call this right after TrackMonocular (or stereo or RGBD)
    int GetTrackingState(); // moved from protected to public

    // TODO: Save/Load functions
    // SaveMap(const string &filename);
    // LoadMap(const string &filename);

    // bool SaveMap(const string &filename);

protected:
    // The `Builder` used to construct this `System`.
    std::unique_ptr<IBuilder> mpBuilder;

    // Information from most recent processed frame
    // You can call this right after TrackMonocular (or stereo or RGBD)
    std::vector<MapPoint*> GetTrackedMapPoints();
    std::vector<cv::KeyPoint> GetTrackedKeyPointsUn();

private:

    bool SetCallStackSize (const rlim_t kNewStackSize);

    rlim_t GetCurrentCallStackSize ();

    bool LoadMap(const string &filename);

    bool currently_localizing_only_;

    bool load_map;

    std::string map_file;

    // Input sensor
    const eSensor mSensor;

    // ORB vocabulary used for place recognition and feature matching.
    ORBVocabulary* mpVocabulary;

    // KeyFrame database for place recognition (relocalization and loop detection).
    KeyFrameDatabase* mpKeyFrameDatabase;

    // Map structure that stores the pointers to all KeyFrames and MapPoints.
    Map* mpMap;

    // Tracker. It receives a frame and computes the associated camera pose.
    // It also decides when to insert a new keyframe, create some new MapPoints and
    // performs relocalization if tracking fails.
    Tracking* mpTracker;

    // Local Mapper. It manages the local map and performs local bundle adjustment.
    LocalMapping* mpLocalMapper;

    // Loop Closer. It searches loops with every new keyframe. If there is a loop it performs
    // a pose graph optimization and full bundle adjustment (in a new thread) afterwards.
    LoopClosing* mpLoopCloser;

    IPublisherThread *mpPublisher;

    // System threads: Local Mapping, Loop Closing, Viewer.
    // The Tracking thread "lives" in the main execution thread that creates the System object.
    bool mbStarted;
    std::thread mtLocalMapping;
    std::thread mtLoopClosing;
    std::thread mtPublisher;
    // std::thread mtCamPublisher;

    // Reset flag
    std::mutex mMutexReset;
    bool mbReset;

    // Change mode flags
    std::mutex mMutexMode;
    bool mbActivateLocalizationMode;
    bool mbDeactivateLocalizationMode;

    // Tracking state
    int mTrackingState;
    std::vector<MapPoint*> mTrackedMapPoints;
    std::vector<cv::KeyPoint> mTrackedKeyPointsUn;
    std::mutex mMutexState;

};

}// namespace ORB_SLAM

#endif // SYSTEM_H
