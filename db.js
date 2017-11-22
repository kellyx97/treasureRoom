// Database
const mongoose = require('mongoose');
var testUser = "test";
var testPass = "test";
var dbURI = "mongodb://" + testUser + ":" + testPass + "@ds117316.mlab.com:17316/happythoughts";

mongoose.connect(dbURI);

mongoose.connection.on('connected', function() {
    console.log("Mongoose connected to: " + dbURI);
});

mongoose.connection.on('error', function(err) {
    console.log("Mongoose connection error: " + err);
});

mongoose.connection.on('disconnected', function() {
    console.log('Mongoose disconnected');
});

// Close app nicely
var gracefulShutdown = function(msg, callback) {
    mongoose.connection.close(function() {
        console.log('Mongoose disconnected through ' + msg);
        callback();
    });
};

// Nodemon restarts
// once because we don't want to repeatedly listen for this, since nodemon is listening too
process.once('SIGUSR2', function(){
    gracefulShutdown('nodemon restart', function() {
        process.kill(process.pid, 'SIGUSR2');
    });
});

// App termination
process.on('SIGINT', function(){
    gracefulShutdown('app termination', function() {
        process.exit(0);
    });
});

// Heroku termination
process.on('SIGTERM', function(){
    gracefulShutdown('Heroku app shutdown', function() {
        process.exit(0);
    });
});

var thoughtSchema = mongoose.Schema({
    text: {type: String, required: true},
    dateTime: {type: String, required: true}
});

mongoose.model('Thought', thoughtSchema, 'thoughts');

module.exports = mongoose;
