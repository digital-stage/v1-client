module.exports = {
    "extends": [
        'airbnb-typescript/base', // For react replace with: airbnb-typescript
        "plugin:promise/recommended",
        "prettier"
    ],
    rules: {
        "prefer-destructuring": ["error", {"object": true, "array": false}]
    },
    "settings": {
        /*"react": {
          "version": "detect"
        }*/
    },
    "parserOptions": {
        "project": './tsconfig.json'
    }
}
