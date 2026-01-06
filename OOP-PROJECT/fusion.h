#ifndef FUSION_H
#define FUSION_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "sensor_reading.h"
#include "sensor.h"
#include "types.h"

using namespace std;

class SensorFusionEngine 
{
    private:
        //Value that checks for very low certainty
        double minConfidenceThreshold;

        // Υπολογισμός σταθμισμένου μέσου όρου
        double weightedAverage(const vector<double>& values, const vector<double>& weights) 
        {
            double sum = 0.0;
            double weightSum = 0.0;

            //Η φόρμουλα είναι: (μετρήσεις * βεβαιότητα) / (άθροισμα βεβαιοτήτων)
            for (size_t i = 0; i < values.size(); i++) 
            {
                sum += values[i] * weights[i];
                weightSum += weights[i];
            }
            
            return (weightSum > 0) ? (sum / weightSum) : 0.0;
        }
        
        // Σταθμισμένος μέσος όρος για Position
        Position weightedAveragePosition(const vector<Position>& positions, const vector<float>& confidences)
        {
            vector<double> xValues, yValues, weights;
            
            for (size_t i = 0; i < positions.size(); i++) 
            {
                xValues.push_back(positions[i].x);
                yValues.push_back(positions[i].y);
                weights.push_back(confidences[i]);
            }
            
            Position final_position;
            final_position.x = weightedAverage(xValues, weights);
            final_position.y = weightedAverage(yValues, weights);
            return final_position;
        }
        
        // Σταθμισμένος μέσος όρος για Direction
        Direction weightedAverageDirection(const vector<Direction>& directions, const vector<float>& confidences)
        {
            vector<double> xValues, yValues, weights;
            
            for (size_t i = 0; i < directions.size(); i++) 
            {
                xValues.push_back(directions[i].x);
                yValues.push_back(directions[i].y);
                weights.push_back(confidences[i]);
            }
            
            Direction final_direction;
            final_direction.x = weightedAverage(xValues, weights);
            final_direction.y = weightedAverage(yValues, weights);
            return final_direction;
        }
        
        // Έλεγχος αν κάποιος αισθητήρας ανίχνευσε ποδήλατο 
        // Η εκφώνηση λέει τα ποδήλατα δεν απορρίπτονται ποτέ για λόγους ασφαλείας
        bool containsBicycle(const vector<SensorReading>& read)
        {
            for (const auto& reading : read) 
            {
                if (reading.type == "BIKE")  return true;
            }
            return false;
        }

    public:
        // Constructor
        SensorFusionEngine(double threshold = 0.5)
            : minConfidenceThreshold(threshold) {}
        
        // Destructor
        ~SensorFusionEngine() {}
        
        // Set confidence threshold
        void setConfidenceThreshold(double threshold) 
        {
            minConfidenceThreshold = threshold;
        }
        
        //fuseSensorData - δέχεται vector από SensorReading
        vector<SensorReading> fuseSensorData(const vector<SensorReading>& allReadings)
        {
            // Αν έχουμε μόνο έναν αισθητήρα, επιστρέφουμε τα δεδομένα του απευθείας
            if (allReadings.size() == 1) 
            {
                vector<SensorReading> result;
                if (allReadings[0].confidence >= minConfidenceThreshold || allReadings[0].type == "BIKE")
                {
                    result.push_back(allReadings[0]);
                }
                return result;
            }
            
            // Βήμα 1: Ομαδοποίηση μετρήσεων ανά objectId
            map<string, vector<SensorReading>> groupedReadings;
            
            for (const auto& reading : allReadings) 
            {
                groupedReadings[reading.objectId].push_back(reading);
            }
            
            // Βήμα 2: Συγχώνευση για κάθε αντικείμενο
            vector<SensorReading> fusedResults;
            
            for (const auto& [objectId, readings] : groupedReadings) 
            {
                // Αν υπάρχει μόνο μία μέτρηση για αυτό το αντικείμενο
                if (readings.size() == 1) 
                {
                    const auto& r = readings[0];
                    // Έλεγχος κατωφλίου
                    if (r.confidence >= minConfidenceThreshold || r.type == "BIKE")
                    {
                        fusedResults.push_back(r);
                    }
                    continue;
                }
                
                // Συγχώνευση πολλαπλών μετρήσεων
                SensorReading fusedReading;
                fusedReading.objectId = objectId;
                
                // Συλλογή τιμών και βεβαιότητας
                vector<Position> positions;
                vector<float> distances;
                vector<float> confidences;
                vector<Direction> directions;
                vector<string> speeds;
                
                string signText = "N/A";
                string lightColour = "N/A";
                string bestType = readings[0].type;
                float maxConf = readings[0].confidence;
                
                for (const auto& r : readings) 
                {
                    // Μόνο έγκυρες θέσεις (όχι -1, -1)
                    if (r.position.x != -1 && r.position.y != -1) 
                        positions.push_back(r.position);
                    
                    // Μόνο έγκυρες αποστάσεις
                    if (r.distance >= 0) 
                        distances.push_back(r.distance);
                    
                    confidences.push_back(r.confidence);
                    
                    // Μόνο έγκυρες κατευθύνσεις
                    if (r.direction.x != 0.0 || r.direction.y != 0.0) 
                        directions.push_back(r.direction);
                    
                    // Συλλογή ταχυτήτων (εκτός από N/A)
                    if (r.speed != "N/A" && !r.speed.empty()) 
                        speeds.push_back(r.speed);
                    
                    // Tύπος με μεγαλύτερη βεβαιότητα
                    if (r.confidence > maxConf) 
                    {
                        maxConf = r.confidence;
                        bestType = r.type;
                    }
                    
                    // Πινακίδες και φώτα (μόνο η κάμερα τα επιστρέφει)
                    if (r.signText != "N/A" && !r.signText.empty())
                        signText = r.signText;

                    if (r.lightColour != "N/A" && !r.lightColour.empty())
                        lightColour = r.lightColour;
                }
                
                fusedReading.type = bestType;

                // Υπολογισμός σταθμισμένων μέσων όρων
                
                // Θέση (σταθμισμένος μέσος όρος)
                if (!positions.empty()) 
                {
                    fusedReading.position = weightedAveragePosition(positions, confidences);
                } 
                else 
                {
                    fusedReading.position.x = -1;
                    fusedReading.position.y = -1;
                }
                
                // Απόσταση (σταθμισμένος μέσος όρος)
                if (!distances.empty()) 
                {
                    vector<double> distValues(distances.begin(), distances.end());
                    vector<double> confWeights(confidences.begin(), confidences.end());
                    fusedReading.distance = weightedAverage(distValues, confWeights);
                }
                else 
                {
                    fusedReading.distance = -1.0f;
                }
                
                // Βεβαιότητα (μέσος όρος)
                float confSum = 0.0f;
                for (float conf : confidences) 
                {
                    confSum += conf;
                }
                //Η φόρμουλα είναι: (μετρήσεις * βεβαιότητα) / (άθροισμα βεβαιοτήτων)                
                fusedReading.confidence = confSum / confidences.size();
                
                // Κατεύθυνση (σταθμισμένος μέσος όρος)
                if (!directions.empty()) 
                {
                    fusedReading.direction = weightedAverageDirection(directions, confidences);
                } 
                else 
                {
                    fusedReading.direction.x = 0.0;
                    fusedReading.direction.y = 0.0;
                }
                
                // Ταχύτητα (χρησιμοποιούμε την πιο συχνή ή την πρώτη διαθέσιμη)
                if (!speeds.empty()) 
                {
                    fusedReading.speed = speeds[0];
                } 
                else 
                {
                    fusedReading.speed = "N/A";
                }
                
                fusedReading.signText = signText;
                fusedReading.lightColour = lightColour;
                
                // ΚΡΙΣΙΜΟΣ ΕΛΕΓΧΟΣ: Απόρριψη χαμηλής βεβαιότητας
                // ΕΞΑΙΡΕΣΗ: Ποδήλατα ΔΕΝ απορρίπτονται (ασφάλεια)
                bool isBicycle = containsBicycle(readings);
                
                if (fusedReading.confidence >= minConfidenceThreshold || isBicycle)
                {
                    fusedResults.push_back(fusedReading);
                }
            }
            
            return fusedResults;
        }
};

#endif // FUSION_H 

/*#ifndef FUSION_H
#define FUSION_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "sensor_reading.h"

using namespace std;

class SensorFusionEngine {
    public:
        unsigned int minConfidencePercent = 40; // default

        SensorFusionEngine(unsigned int minConf = 40) : minConfidencePercent(minConf) {}

        // Main fusion function:
        // Takes a list of sensor readings, groups them by objectId,
        // and combines data using confidence-weighted averaging.
        vector<SensorReading> fuseSensorData(const vector<SensorReading>& readings) const
        {
            // Step 1: Group readings by objectId.
            // If a reading has no ID, group it under "N/A".
            map<string, vector<SensorReading>> grouped;
            for (const auto& r : readings) {
                string id = r.objectId;
                if (id.empty()) id = "N/A"; // group unknowns together
                grouped[id].push_back(r);
            }
            // Step 2: Process each group to create a fused reading.
            vector<SensorReading> out;
            for (const auto& kv : grouped) {
                const string& id = kv.first;
                const auto& group = kv.second;

                // Initialize a fused reading with default values.
                float wsum = 0.0f;
                SensorReading fused;
                fused.objectId = id;
                fused.type = "N/A";
                fused.position = Position{0,0};
                fused.distance = 0.0f;
                fused.confidence = 0.0f;
                fused.speed = "N/A";
                fused.signText = "N/A";
                fused.lightColour = "N/A";

                // For type and categorical fields we pick the highest-confidence observation
                float bestTypeConf = -1.0f;
                // Step 3: Fuse readings within the group.
                for (const auto& r : group) {
                    float c = r.confidence;
                    if (c > bestTypeConf && !r.type.empty()) {
                        fused.type = r.type;
                        bestTypeConf = c;
                    }
                    // accumulate weighted position/distance
                    fused.position.x += static_cast<int>(r.position.x * c);
                    fused.position.y += static_cast<int>(r.position.y * c);
                    fused.distance += r.distance * c;
                    fused.confidence += c;
                    wsum += c;
                    if (!r.speed.empty() && fused.speed == "N/A") fused.speed = r.speed;
                    if (!r.signText.empty() && fused.signText == "N/A") fused.signText = r.signText;
                    if (!r.lightColour.empty() && fused.lightColour == "N/A") fused.lightColour = r.lightColour;
                }
                // Step 4: Normalize by total weights to get weighted averages.
                if (wsum > 0.0f) {
                    fused.position.x = static_cast<int>(fused.position.x / wsum);
                    fused.position.y = static_cast<int>(fused.position.y / wsum);
                    fused.distance = fused.distance / wsum;
                    fused.confidence = fused.confidence / static_cast<float>(group.size());
                }

                // Convert to percent
                float confPercent = fused.confidence * 100.0f;

                // Keep reading if above threshold, or if type indicates bicycle (safety rule)
                if (confPercent >= static_cast<float>(minConfidencePercent) || fused.type == "BIKE") {
                    out.push_back(fused);
                }
            }
            return out;
        }
};

#endif // FUSION_H */
