const express = require('express');
const path = require('path');
const fs = require('fs-extra');

const app = express();
const PORT = 3000;

// Serve static files
app.use(express.static('public'));

// API endpoint to get Muzeel analysis report
app.get('/api/report', async (req, res) => {
    try {
        const report = await fs.readJson('./muzeel-report.json');
        res.json(report);
    } catch (error) {
        res.json({ error: 'Run muzeel-core.js first' });
    }
});

// API endpoint to compare versions
app.get('/api/compare', async (req, res) => {
    const original = await fs.readFile('./public/original.js', 'utf8');
    const optimized = await fs.readFile('./public/muzeel-optimized.js', 'utf8');
    
    res.json({
        original: { size: original.length, lines: original.split('\n').length },
        optimized: { size: optimized.length, lines: optimized.split('\n').length },
        savings: {
            size: original.length - optimized.length,
            percentage: ((1 - optimized.length/original.length) * 100).toFixed(1)
        }
    });
});

app.listen(PORT, () => {
    console.log(`
    MUZEEL DEMO - JavaScript Dead Code Eliminator          
    Server running at: http://localhost:${PORT}      
    First, run: npm run analyze, Then visit the URL above                       
    `);
});