#pragma once

#include <sys/time.h>
#include <time.h>
#include <chrono>
#include <memory>

#include "random_var.h"

namespace NRTSimulator
{

	void * runTask(void * params);

	class TTask
	{
	private:
		TRandomVar ExecutionTime;
		TRandomVar Period;

		std::chrono::time_point<std::chrono::high_resolution_clock> EndSimulation;
		std::chrono::time_point<std::chrono::high_resolution_clock> NextJobFire;

		std::chrono::nanoseconds WorstCaseResponce;

		struct timespec JobFireTimeSpec;

		int CPU;
		int Priority;
		std::string Name;

		std::vector<std::chrono::nanoseconds> ResponceTimes;

		pthread_t ThreadId;
	public:
		TTask(const TRandomVar & executionTime, const TRandomVar & period, int cpu,
		      int priority,
		      const std::string & name);
		void Run(long long startAt, long long endAt);
		void Join();

		int GetCpu() const;
		int GetPriority() const;
		long long GetWorstCaseExecutionTime() const;
		const std::string & GetName() const;
		long long GetPeriod() const;

		long long GetWorstCaseResponceTime() const;

		std::vector<long long> GetResponceTimes() const;

		virtual ~TTask();
	protected:
		virtual void Initialize();
	private:
		void ComputeFireTimerSpec();

		void TaskBody();

		void SetUpCPU();

		void SetUpPriority();

		void WaitForNextActivation();

		virtual void JobBody(long long executionTime) = 0;
		friend void * runTask(void * params);
	};

	class TCountingTask : public TTask
	{
	private:
		static double
		ConvertRate; //ConvertRate how many ns one "long long" addition take. (Around 3.69103546)
	public:
		TCountingTask(const TRandomVar & executionTime, const TRandomVar & period,
		              int cpu,
		              int priority, const std::string & name);
		virtual ~TCountingTask();
		static void EstimateConvertRate();
	private:
		virtual void JobBody(long long) override;
	};

	class TTimerTask : public TTask
	{
	private:
		struct timespec JobDoneTimeSpec;
		struct timespec JobStartCPUClockTimeSpec;
	public:
		TTimerTask(const TRandomVar & executionTime, const TRandomVar & period, int cpu,
		           int priority, const std::string & name);
		virtual ~TTimerTask();
	private:
		virtual void JobBody(long long) override;
	};

}