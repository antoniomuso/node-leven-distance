
module.exports = (a, b) => {
    if (a === b) {
        return 0
    }
    var leven = require('./build/Release/addon.node').levenshtein_distance
    return leven(a,b)
}
