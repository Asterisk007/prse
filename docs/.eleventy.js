module.exports = function(eleventyConfig){
    eleventyConfig.addPassthroughCopy("assets");

    return {
        passthroughFileCopy: true,
        markdownTemplateEngine: "njk",
        templateFormats: ["html", "njk"],
        dir: {
            input: "src",
            output: "../public/",
            includes: "includes"
        }
    }
};