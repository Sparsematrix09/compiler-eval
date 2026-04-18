//contains dead code
window.functionTracker = {
    functions: [],
    total: 0,
    used: 0,
    dead: 0,
    track: function(funcName) {
        const existing = this.functions.find(f => f.name === funcName);
        if (existing && !existing.used) {
            existing.used = true;
            this.used++;
            this.dead--;
            console.log(` USED: ${funcName} (marked as used)`);
            if (window.updateStats) window.updateStats();
        }
    },
    addFunction: function(funcName) {
        if (!this.functions.find(f => f.name === funcName)) {
            this.functions.push({ name: funcName, used: false });
            this.total++;
            this.dead++;
            console.log(`📝 Registered: ${funcName} (pending execution)`);
            if (window.updateStats) window.updateStats();
        }
    }
};

// used functions

function handleClick() {
    window.functionTracker.track('handleClick');
    const output = document.getElementById('output');
    output.innerHTML = `Button clicked at ${new Date().toLocaleTimeString()}`;
    output.style.background = '#c6f6d5';
    console.log('handleClick executed - showing success message');
}

function handleHover() {
    window.functionTracker.track('handleHover');
    const output = document.getElementById('output');
    output.innerHTML = `Mouse entered at ${new Date().toLocaleTimeString()}`;
    output.style.background = '#fed7d7';
    console.log('handleHover executed - hover effect triggered');
}

function handleHoverOut() {
    window.functionTracker.track('handleHoverOut');
    const output = document.getElementById('output');
    output.innerHTML = ` Mouse left at ${new Date().toLocaleTimeString()}`;
    output.style.background = '#feebc8';
    console.log('handleHoverOut executed - hover out effect');
}

function showAlert() {
    window.functionTracker.track('showAlert');
    alert('This is a used function!');
    console.log('showAlert executed - alert shown to user');
}

function calculateSum(a, b) {
    window.functionTracker.track('calculateSum');
    const sum = a + b;
    const output = document.getElementById('output');
    output.innerHTML = ` Sum of ${a} + ${b} = ${sum}`;
    output.style.background = '#e9d8fd';
    console.log(`calculateSum executed - result: ${sum}`);
    return sum;
}

// dead cod

function unusedFunction1() {
    // This function is NEVER called - Muzeel will eliminate it
    window.functionTracker.track('unusedFunction1');
    console.log('This will never be logged!');
    return "I'm dead code!";
}

function unusedFunction2(param1, param2) {
    // Another dead function - complex logic that never runs
    window.functionTracker.track('unusedFunction2');
    let result = 0;
    for (let i = 0; i < 1000; i++) {
        result += i * param1;
    }
    return result * param2;
}

function unusedAnalyticsTracker() {
    // Pretend this tracks user behavior - but never used
    window.functionTracker.track('unusedAnalyticsTracker');
    const data = {
        page: window.location.href,
        timestamp: Date.now(),
        userAgent: navigator.userAgent
    };
    console.log('Analytics would be sent:', data);
    return data;
}

function unusedDOMManipulator() {
    // Would manipulate DOM but never called
    window.functionTracker.track('unusedDOMManipulator');
    const newDiv = document.createElement('div');
    newDiv.innerHTML = 'This element would be added if this function was used';
    newDiv.style.padding = '10px';
    newDiv.style.background = 'yellow';
    document.body.appendChild(newDiv);
}

function unusedHeavyComputation() {
    // Heavy computation that never runs - waste of bundle size
    window.functionTracker.track('unusedHeavyComputation');
    let fib = [0, 1];
    for (let i = 2; i < 100; i++) {
        fib.push(fib[i-1] + fib[i-2]);
    }
    return fib;
}

// Register all functions for tracking
const allFunctions = [
    'handleClick', 'handleHover', 'handleHoverOut', 'showAlert', 'calculateSum',
    'unusedFunction1', 'unusedFunction2', 'unusedAnalyticsTracker', 
    'unusedDOMManipulator', 'unusedHeavyComputation'
];

allFunctions.forEach(func => window.functionTracker.addFunction(func));

console.log(' Muzeel Demo Loaded!');
console.log( allFunctions.length + ' functions registered');
console.log( window.functionTracker.dead + ' functions are dead code (will be eliminated)');