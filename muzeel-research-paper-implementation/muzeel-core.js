#!/usr/bin/env node

/**
 * Muzeel Core - JavaScript Dead Code Detection & Elimination
 * 
 * This script demonstrates the core algorithm of Muzeel:
 * 1. Parse JavaScript to identify all functions
 * 2. Simulate user interactions to track which functions are called
 * 3. Generate optimized version without dead code
 */

const fs = require('fs-extra');
const path = require('path');

// Configuration
const CONFIG = {
    originalJSPath: './public/original.js',
    optimizedJSPath: './public/muzeel-optimized.js',
    outputReportPath: './muzeel-report.json'
};

// Function to extract all function names from JavaScript code
function extractFunctions(code) {
    const functions = [];
    
    // Regex patterns to match different function declarations
    const patterns = [
        /function\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*\(/g,  // function name()
        /const\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*=\s*function\s*\(/g,  // const name = function()
        /let\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*=\s*function\s*\(/g,   // let name = function()
        /var\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*=\s*function\s*\(/g,    // var name = function()
        /([a-zA-Z_$][a-zA-Z0-9_$]*)\s*:\s*function\s*\(/g,  // object method
        /async\s+function\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*\(/g  // async function
    ];
    
    for (const pattern of patterns) {
        let match;
        while ((match = pattern.exec(code)) !== null) {
            if (!functions.includes(match[1])) {
                functions.push(match[1]);
            }
        }
    }
    
    return functions;
}

// Simulate user interactions (simplified version of Muzeel's bot)
function simulateInteractions() {
    console.log('\n Muzeel: Simulating user interactions...');
    
    // In a real implementation, this would use Puppeteer to actually click/hover
    // For demo, we define which functions are triggered by interactions
    const usedFunctions = [
        'handleClick',
        'handleHover', 
        'handleHoverOut',
        'showAlert',
        'calculateSum'
    ];
    
    console.log(` Triggered ${usedFunctions.length} events`);
    usedFunctions.forEach(f => console.log(`      - ${f}`));
    
    return usedFunctions;
}

// Analyze dead code by comparing all functions vs used functions
function analyzeDeadCode(allFunctions, usedFunctions) {
    const usedSet = new Set(usedFunctions);
    
    const deadFunctions = allFunctions.filter(f => !usedSet.has(f));
    const usedFunctionsList = allFunctions.filter(f => usedSet.has(f));
    
    return {
        total: allFunctions.length,
        used: usedFunctionsList.length,
        dead: deadFunctions.length,
        usedFunctions: usedFunctionsList,
        deadFunctions: deadFunctions,
        eliminationRate: allFunctions.length > 0 
            ? (deadFunctions.length / allFunctions.length * 100).toFixed(2)
            : 0
    };
}

// Generate optimized JavaScript by removing dead functions
function generateOptimizedJS(originalCode, deadFunctions) {
    let optimizedCode = originalCode;
    
    // Remove each dead function (simplified)
    for (const deadFunc of deadFunctions) {
        // Pattern to match entire function declarations
        const patterns = [
            new RegExp(`function\\s+${deadFunc}\\s*\\([^)]*\\)\\s*\\{[\\s\\S]*?\\n\\}`, 'g'),
            new RegExp(`const\\s+${deadFunc}\\s*=\\s*function\\s*\\([^)]*\\)\\s*\\{[\\s\\S]*?\\n\\}`, 'g'),
            new RegExp(`let\\s+${deadFunc}\\s*=\\s*function\\s*\\([^)]*\\)\\s*\\{[\\s\\S]*?\\n\\}`, 'g'),
            new RegExp(`var\\s+${deadFunc}\\s*=\\s*function\\s*\\([^)]*\\)\\s*\\{[\\s\\S]*?\\n\\}`, 'g')
        ];
        
        for(const pattern of patterns){
            optimizedCode = optimizedCode.replace(pattern, '');
        }
    }
    
    // Clean up extra newlines
    optimizedCode = optimizedCode.replace(/\n\s*\n\s*\n/g, '\n\n');
    
    return optimizedCode;
}

// Generate HTML report
function generateReport(analysis) {
    const report = {
        timestamp: new Date().toISOString(),
        analysis: analysis,
        summary: {
            totalFunctions: analysis.total,
            deadFunctionsCount: analysis.dead,
            usedFunctionsCount: analysis.used,
            deadCodePercentage: analysis.eliminationRate,
            estimatedSavings: `${(analysis.dead / analysis.total * 100).toFixed(1)}% code reduction`
        },
        deadFunctionsList: analysis.deadFunctions,
        usedFunctionsList: analysis.usedFunctions
    };
    
    return report;
}

// Main execution
async function main() {
    console.log('\n' + '='.repeat(60));
    console.log(' MUZEEL - JavaScript Dead Code Analyzer');
    console.log('='.repeat(60));
    
    // Step 1: Read original JavaScript
    console.log('\n Step 1: Reading original JavaScript...');
    const originalJS = await fs.readFile(CONFIG.originalJSPath, 'utf8');
    console.log(`   Loaded ${originalJS.length} bytes`);
    
    // Step 2: Extract all functions
    console.log('\n Step 2: Extracting all functions...');
    const allFunctions = extractFunctions(originalJS);
    console.log(`    Found ${allFunctions.length} functions:`);
    allFunctions.forEach(f => console.log(`      - ${f}`));
    
    // Step 3: Simulate user interactions (Muzeel's bot)
    console.log('\n Step 3: Emulating user interactions...');
    const usedFunctions = simulateInteractions();
    
    // Step 4: Analyze dead code
    console.log('\n Step 4: Analyzing dead code...');
    const analysis = analyzeDeadCode(allFunctions, usedFunctions);
    console.log(`   Statistics:`);
    console.log(`      - Total functions: ${analysis.total}`);
    console.log(`      - Used functions: ${analysis.used}`);
    console.log(`      - Dead functions: ${analysis.dead}`);
    console.log(`      - Dead code rate: ${analysis.eliminationRate}%`);
    
    // Step 5: Generate optimized version
    console.log('\n Step 5: Generating optimized JavaScript...');
    const optimizedJS = generateOptimizedJS(originalJS, analysis.deadFunctions);
    await fs.writeFile(CONFIG.optimizedJSPath, optimizedJS);
    console.log(`    Saved to ${CONFIG.optimizedJSPath}`);
    console.log(`    Size reduction: ${originalJS.length} → ${optimizedJS.length} bytes (${((1 - optimizedJS.length/originalJS.length)*100).toFixed(1)}% reduction)`);
    
    // Step 6: Generate report
    console.log('\n Step 6: Generating report...');
    const report = generateReport(analysis);
    await fs.writeJson(CONFIG.outputReportPath, report, { spaces: 2 });
    console.log(`    Report saved to ${CONFIG.outputReportPath}`);
    
    // Final summary
    console.log('\n' + '='.repeat(60));
    console.log(' MUZEEL ANALYSIS COMPLETE!');
    console.log('='.repeat(60));
    console.log(`\n SUMMARY:`);
    console.log(`    Dead functions eliminated: ${analysis.dead}`);
    console.log(`    Used functions retained: ${analysis.used}`);
    console.log(`    Code reduction: ${analysis.eliminationRate}%`);
    console.log(`\n Next steps:`);
    console.log(`   1. Run 'npm start' to see the demo`);
    console.log(`   2. Compare original.js vs muzeel-optimized.js`);
    console.log(`   3. Interact with buttons to see which functions are used`);
    
    // Print dead functions
    if (analysis.deadFunctions.length > 0) {
        console.log(`\n DEAD FUNCTIONS ELIMINATED:`);
        analysis.deadFunctions.forEach(f => console.log(`   - ${f}`));
    }
}

// Run the analysis
main().catch(console.error);