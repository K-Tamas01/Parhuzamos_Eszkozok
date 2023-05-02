const error = require('../../../Response/Error');
const success = require('../../../Response/Success');

const { isnum, range } = require('../../../Root_Service/Service');

module.exports = {
	name:'remove',
	category:'Service',
	description:'$remove index',
	isload: true,
	run: async (bot, msg, args, guildqueue, user) => {
		if (msg.guild.members.me.voice.channel !== null || msg.member.voice.channel !== null) {
			if (guildqueue !== undefined && guildqueue !== null) {
				if (args.length !== 0) {
					if (isnum(args)) {
						if (range(guildqueue.songs.length, args)) {
							success.run(msg, 2, guildqueue, parseInt(args), user);
							guildqueue.remove(parseInt(args));
						}
						else {
							error.run(msg, 8, guildqueue, args);
						}
					}
					else {
						error.run(msg, 7, guildqueue, args);
					}
				}
				else {
					error.run(msg, 5, guildqueue, args);
				}
			}
			else {
				error.run(msg, 4, guildqueue, args);
			}
		}
		else {
			error.run(msg, 2, guildqueue, args);
		}
	},
};