exports.handler = async function (event, context) {
  const key = process.env.SUBSCRIPTION_KEY;

  // return to check
  return {
    statusCode: 200,
    body: `Key is hidden: ${key}`
  };
};

