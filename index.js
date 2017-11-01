var leven = require('./build/Release/addon.node').levenshtein_distance
module.exports = (a, b) => {
    if (a === b) {
        return 0
    }
    return leven(a,b)
}
