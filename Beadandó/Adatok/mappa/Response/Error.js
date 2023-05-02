const { EmbedBuilder } = require('discord.js');

module.exports = {
	run:async (msg, code, errorMessage) => {
		switch (code) {
		case -1:{
			const content = new EmbedBuilder()
				.setTitle(errorMessage)
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 0:{
			const content = new EmbedBuilder()
				.setTitle('Ismeretlen parancs nézd meg a $help-et')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 1:{
			const content = new EmbedBuilder()
				.setTitle('Nem várt argomentum van a sorban!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 2:{
			const content = new EmbedBuilder()
				.setTitle('Nem vagy bent egy Voice csatornában semm!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 3:{
			const content = new EmbedBuilder()
				.setTitle('Nem vagyunk egy Voice csatornában!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 4:{
			const content = new EmbedBuilder()
				.setTitle('Nincs semmi a listában!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 5:{
			const content = new EmbedBuilder()
				.setTitle('Üres argumentum lista nincs elem a sorban!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 6:{
			const content = new EmbedBuilder()
				.setTitle('Egy szám már be van loop-va előbb kapcsold azt ki ($s-loop)!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 7:{
			const content = new EmbedBuilder()
				.setTitle('Hibás paraméter a sorban nem ezt vártam!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 8:{
			const content = new EmbedBuilder()
				.setTitle('A megadott index kivül esik a listán!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 9:{
			const content = new EmbedBuilder()
				.setTitle('A lejátszási már be van loop-olva előbb azt kapcsold ki (q-loop)!')
				.setColor('#FF0000')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		}
	},
};