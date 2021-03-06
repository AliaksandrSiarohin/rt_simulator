#include "tasks_file_parser.h"


#include <iostream>
#include <libconfig.h++>
#include <algorithm>


namespace NRTSimulator
{
	TTaskFileParser::TTaskFileParser(bool isTimerTasks)
		:	IsTimerTasks(isTimerTasks)
	{
	}

	TRandomVar TTaskFileParser::ParseRandomVar(const libconfig::Setting & probs,
	        const std::string name)
	{
		std::vector<double> probMasses;
		std::vector<long long> probTimes;

		size_t numberOfProbs = probs.getLength();

		for (size_t probNumber = 0; probNumber < numberOfProbs; ++ probNumber) {
			double mass;

			if (!probs[probNumber].lookupValue("mass", mass)) {
				std::cerr << "No 'mass' in " << name <<
				          " (It soubld be double with decimal point)" << std::endl;
				exit(-1);
			}

			long long t;

			if (!probs[probNumber].lookupValue("time", t)) {
				std::cerr << "No 'time' in " << name << " (It should be long with L)"
				          << std::endl;
				exit(-1);
			}

			probMasses.push_back(mass);
			probTimes.push_back(t);
		}
		if (std::abs(std::accumulate(probMasses.begin(), probMasses.end(),
		                             0.0) - 1) > 0.001) {
			std::cerr << "Probabilities in " << name << " don`t sum up to 1" << std::endl;
			exit(-1);
		}

		return TRandomVar(probMasses, probTimes);
	}

	std::vector<std::shared_ptr<TTask>> TTaskFileParser::Parse(
	                                     const std::string & file)
	{
		std::vector<std::shared_ptr<TTask>> result;

		libconfig::Config cfg;

		try {
			cfg.readFile(file.c_str());
		} catch (const libconfig::FileIOException &fioex) {
			std::cerr << "I/O error while reading file." << std::endl;
			exit(-1);
		} catch (const libconfig::ParseException &pex) {
			std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			          << " - " << pex.getError() << std::endl;
			exit(-1);
		}


		try {
			const libconfig::Setting& tasks = cfg.getRoot()["tasks"];
			size_t numberOfTasks = tasks.getLength();

			for (size_t taskNumber = 0; taskNumber < numberOfTasks; ++taskNumber) {
				const libconfig::Setting& task = tasks[taskNumber];
				std::string name;

				if (!task.lookupValue("name", name)) {
					std::cerr << "No 'name' in " << taskNumber << " task" << std::endl;
					exit(-1);
				}

				int cpu;

				if (!task.lookupValue("cpu", cpu)) {
					std::cerr << "No 'cpu' in " << name << std::endl;
					exit(-1);
				}

				int priority;

				if (!task.lookupValue("priority", priority)) {
					std::cerr << "No 'priority' in " << name << std::endl;
					exit(-1);
				}

				TRandomVar execution = ParseRandomVar(task["execution"], name);
				TRandomVar period = ParseRandomVar(task["period"], name);

				if (IsTimerTasks) {
					result.push_back(std::shared_ptr<TTask>
					                 (new TTimerTask(execution, period , cpu, priority, name)));
				} else {
					result.push_back(std::shared_ptr<TTask>
					                 (new TTimerTask(execution, period , cpu, priority, name)));
				}

			}
		} catch (const libconfig::SettingNotFoundException &nfex) {
			std::cerr << "No " << nfex.getPath() << " setting in configuration file." <<
			          std::endl;
			exit(-1);
		}

		return result;
	}
}