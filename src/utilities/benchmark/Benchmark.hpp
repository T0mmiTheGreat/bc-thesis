/**
 * @file Benchmark.hpp
 * @author Tomáš Ludrovan
 * @brief Benchmark class
 * @version 0.1
 * @date 2024-05-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

class Benchmark {
private:
	Benchmark() {}
	~Benchmark() {
		for (auto& [id, rec] : m_records) {
			rec->stream << std::string(40, '-') << std::endl;
			rec->stream << (rec->sum / rec->recCnt) << std::endl;
		}
	}
	// Copy constructor - disable
	Benchmark(Benchmark& other) = delete;
	// Copy assignment operator - disable
	Benchmark& operator=(Benchmark& other) = delete;
public:
	/**
	 * @brief Returns instance of the Benchmark singleton.
	 */
	static Benchmark& get() {
		static Benchmark instance; // Scott Meyer's Singleton
		return instance;
	}
private:
	typedef std::chrono::duration<double, std::milli> Duration;
	typedef std::string LogId;

	struct Record {
		std::ofstream stream;
		std::chrono::time_point<std::chrono::high_resolution_clock> t0;
		double sum;
		int recCnt;

		Record(const LogId& id)
			: stream(id + ".log")
			, t0{std::chrono::high_resolution_clock::now()}
			, sum{0.0}
			, recCnt{0}
		{}
	};
	typedef std::unique_ptr<Record> RecordP;

	typedef std::unordered_map<LogId, RecordP> Records;

	Records m_records;

	RecordP& getRecord(const LogId& id) {
		auto& res = m_records[id];

		if (res == nullptr) {
			res = std::make_unique<Record>(id);
		}

		return res;
	}
public:
	void beginMeasure(const LogId& id) {
		auto& rec = getRecord(id);
		rec->t0 = std::chrono::high_resolution_clock::now();
	}
	void endMeasure(const LogId& id) {
		auto t1 = std::chrono::high_resolution_clock::now();

		auto& rec = getRecord(id);

		std::chrono::duration<double, std::milli> ms = t1 - rec->t0;
		double msCnt = ms.count();

		rec->sum += msCnt;
		rec->recCnt++;

		rec->stream << ms.count() << std::endl;
	}
};

#endif // BENCHMARK_HPP
