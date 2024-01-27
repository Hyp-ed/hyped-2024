/** @type {import("eslint").Linter.Config} */
module.exports = {
  extends: [
    'eslint:recommended',
    'eslint-config-turbo',
    'plugin:@typescript-eslint/recommended',
    'plugin:@typescript-eslint/recommended-requiring-type-checking',
    'prettier',
  ],
  parser: '@typescript-eslint/parser',
  plugins: ['only-warn', '@typescript-eslint'],
  ignorePatterns: [
    '**/dist/**/*',
    '**/node_modules/**/*',
    '.eslintrc.js',
    '.eslintrc.cjs',
  ],
  rules: {
    'no-console': 'error',
  },
  root: true,
};
