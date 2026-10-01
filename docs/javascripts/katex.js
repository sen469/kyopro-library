document$.subscribe(() => {
  document.querySelectorAll(".arithmatex").forEach((element) => {
    renderMathInElement(element, {
      delimiters: [
        { left: "\\[", right: "\\]", display: true },
        { left: "\\(", right: "\\)", display: false }
      ],
      throwOnError: false
    });
  });
});
