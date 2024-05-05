/**
 * @file Benchmark.hpp
 * @author Tomáš Ludrovan
 * @brief Classes for benchmarking
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

/**
 * @brief Singleton class for benchmarking.
 */
class Benchmark {
private:
	Benchmark() {}
	~Benchmark() {
		// When program stops, write the average values at the end of the log.
		for (auto& [id, rec] : m_records) {
			rec->stream << std::string(40, '-') << std::endl; // 40 dashes
			rec->stream << (rec->sum / rec->recCnt) << std::endl; // avg
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

	// Log identifier. Also, should be a valid filename.
	typedef std::string LogId;
private:
	typedef std::chrono::duration<double, std::milli> Duration;

	/**
	 * @brief A single log.
	 */
	struct Record {
		std::ofstream stream;
		std::chrono::time_point<std::chrono::high_resolution_clock> t0;
		double sum;
		int recCnt;

		/**
		 * @brief Constructs a new Record object.
		 * 
		 * @param id Name of the log.
		 */
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

	/**
	 * @brief Return a record matching the provided `id`.
	 * 
	 * @details Searches for a record in the `m_records` map. If no record
	 *          with the provided `id` is found, creates a new one.
	 */
	RecordP& getRecord(const LogId& id) {
		// Find or create
		auto& res = m_records[id];

		if (res == nullptr) {
			// Not found -- initialize

			res = std::make_unique<Record>(id);
		}

		return res;
	}
public:
	/**
	 * @brief Starts a new process of measuring.
	 * 
	 * @param id ID of the measuring.
	 */
	void beginMeasure(const LogId& id) {
		auto& rec = getRecord(id);
		rec->t0 = std::chrono::high_resolution_clock::now();
	}
	/**
	 * @brief Stops a process of measuring and writes the results to the log
	 *        file.
	 * 
	 * @param id ID of the measuring.
	 */
	void endMeasure(const LogId& id) {
		// Save the timestamp when the method is called, because other
		// operations take some time too.
		auto t1 = std::chrono::high_resolution_clock::now();

		auto& rec = getRecord(id);

		// Milliseconds taken
		std::chrono::duration<double, std::milli> ms = t1 - rec->t0;
		// ms converted to double
		double msCnt = ms.count();

		// For calculating the average
		rec->sum += msCnt;
		rec->recCnt++;

		// Output to stream
		rec->stream << ms.count() << std::endl;
	}
	/**
	 * @brief Returns a stream associated with a measuring.
	 * 
	 * @param id ID of the measuring.
	 */
	std::ostream& getStream(const LogId& id) {
		return m_records[id]->stream;
	}
};

/**
 * @brief A class whose lifetime is equal to a single measure.
 * 
 * @details Create instance of this class at the point where you want to start
 *          measuring, and once the instance goes out of scope, it will
 *          automatically end the measuring.
 */
class AutoLogger_Measure {
private:
	std::string m_benchId;
public:
	/**
	 * @brief Constructs a new AutoLogger object.
	 * 
	 * @param benchId ID of the measuring.
	 */
	AutoLogger_Measure(const std::string& benchId)
		: m_benchId{benchId}
	{
		Benchmark::get().beginMeasure(benchId);
	}
	virtual ~AutoLogger_Measure() {
		Benchmark::get().endMeasure(m_benchId);
	}
};

/**
 * @brief A class whose lifetime is equal to a single measure.
 * 
 * @details Create instance of this class at the point where you want to start
 *          measuring, and once the instance goes out of scope, it will
 *          automatically end the measuring.
 */
class AutoLogger_NodesSqdist {
private:
	std::string m_benchId;
	size_t m_nodeCount;
	double m_dist;
public:
	/**
	 * @brief Constructs a new AutoLogger_NodesSqdist object.
	 * 
	 * @param benchId ID of the measuring.
	 * @param dist Initial squared distance from the goal.
	 */
	AutoLogger_NodesSqdist(const std::string& benchId, double dist)
		: m_benchId{benchId}
		, m_nodeCount{0}
		, m_dist{dist}
	{}
	~AutoLogger_NodesSqdist() {
		// Output "(%d nodes, %f sqdist)"
		Benchmark::get().getStream(m_benchId)
			<< "(" << m_nodeCount << " nodes, " << m_dist << " sqdist)"
			<< std::endl;
	}
	/**
	 * @brief Increments the node counter by 1.
	 */
	inline void incNodeCount() { m_nodeCount++; }
};

#endif // BENCHMARK_HPP
